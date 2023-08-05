#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <poll.h>
#include <vector>
#include "Tintin_reporter.hpp"
#include <cstring>

class Server {
    public:
        Server(int port);
        ~Server();
        void run(bool *terminated);

    private:
        int _port;
        int _serverFd;
        std::vector<struct pollfd> _polls;

        int createSocket() const;
        int setOptions() const;
        int bindSocket() const;
        int acceptSocket() const;
        int listenSocket() const;
};
#endif