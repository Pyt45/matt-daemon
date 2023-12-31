#include "../include/server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#define BUFFER_SIZE 1024

Server::Server(int port): _port(port) {
}

Server::~Server() {
    for (size_t i = 0; i < this->_polls.size(); i++) {
        if (this->_polls[i].fd > 0) {
            close(this->_polls[i].fd);
        }
    }
}

void Server::run(bool *terminated) {
    int serverFd = this->createSocket();

    if (serverFd < 0) {
        throw new std::runtime_error("Error: failed to create a socket");
    }
    this->_serverFd = serverFd;


    if (this->setOptions()) {
        throw new std::runtime_error("Error: occured while setting up the options");
    }

    if (this->bindSocket() < 0) {
        throw new std::runtime_error("Error: occured while binding the socket");
    }

    if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) != 0) {
        throw new std::runtime_error("Error: occured while setting descriptor status to nonblock");
    }

    if (this->listenSocket() > 0) {
        throw new std::runtime_error("Error: Server is not started listening");
    }

    this->_polls.push_back((struct pollfd){this->_serverFd, POLLIN, 0});

    std::vector<struct pollfd> newSocks;
    std::set<int> toRemove;


    bool exit = false;


    while (!exit && !*terminated) {
            int n = poll(&this->_polls[0], this->_polls.size(), 10000);

            if (n == 0)
                continue;
            else if (n == -1) {
                *terminated = true;
            }
            for (size_t i = 0; i < this->_polls.size(); i++) {
                if (i == 0) {
                    if (this->_polls[i].revents & POLLIN) {
                        int fd = this->acceptSocket();
                    
                        if (fd > 0 && this->_polls.size() == 4) {
                            Tintin_reporter::getLogger().log(ERR_SERVER, ERROR, "server can't accept this connection");
                            close(fd);
                        } else if (fd > 0) {
                            this->_polls[i].events = POLLIN;
                            newSocks.push_back((struct pollfd){fd, POLLIN, 0});
                        } else {
                            Tintin_reporter::getLogger().log(ERR_SERVER, ERROR, "failed to accept socket");
                        }
                    }
                } else if (this->_polls[i].revents & POLLIN) {
                    char buffer[BUFFER_SIZE + 1];
                    int n = recv(this->_polls[i].fd, buffer, BUFFER_SIZE, 0);
                    if (n == -1) {

                    }
                    else if (n == 0) {
                        toRemove.insert(this->_polls[i].fd);
                        close(this->_polls[i].fd);
                        continue;
                    } else {
                        buffer[n] = 0;

                        char *token = strtok(buffer, "\n\r");
                        while( token != NULL ) {
                            if (strcmp(token, "quit") == 0) {
                                exit = true;
                                *terminated = true;
                                Tintin_reporter::getLogger().log(REQUEST_QUIT, INFO, "");
                                break;
                            }

                            Tintin_reporter::getLogger().log(USER_INPUT, LOG, token);
                            token = strtok(NULL, "\n\r");
                        }
                    }
                } else if (this->_polls[i].revents & POLLHUP) {
                    toRemove.insert(this->_polls[i].fd);
                }
            }

            std::vector<struct pollfd>::iterator it = this->_polls.begin();

            while (it != this->_polls.end()) {
                if (toRemove.find(it->fd) != toRemove.end()) {
                    it = this->_polls.erase(it);
                } else {
                    it++;
                }
            }

            for (size_t i = 0; i < newSocks.size(); i++) {
                this->_polls.push_back((struct pollfd){newSocks[i].fd, POLLIN, 0});
            }
            
            newSocks.clear();
            toRemove.clear();
    }
}

int Server::createSocket() const {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int Server::setOptions() const {
    int opt = 1;
    return setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
}

int Server::bindSocket() const {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(this->_port);
    address.sin_addr.s_addr = INADDR_ANY;

    return bind(this->_serverFd, (struct sockaddr*)&address,
             sizeof(address));
}

int Server::acceptSocket() const {
    struct sockaddr_in my_addr;
    socklen_t addr_size = sizeof(my_addr);
    return accept(this->_serverFd, (struct sockaddr *)&my_addr, &addr_size);
}

int Server::listenSocket() const {
    return listen(this->_serverFd, 5);
}

Server::Server() {}

Server::Server(const Server& rhs) {
    *this = rhs;
}

Server& Server::operator=(const Server& rhs) {
    if (this != &rhs) {
        this->_port = rhs._port;
    }
    return *this;
}