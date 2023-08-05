#include "../include/Matt_daemon.hpp"

Daemon::Daemon() {}

void Daemon::check_pid() {
    if (process_id < 0)
        exit(1);
    if (process_id > 0)
        exit(0);
}

Daemon::Daemon(const Daemon& src) {
    *this = src;
}

Daemon& Daemon::operator=(const Daemon& src) {
    if (this != &src) {
        
    }
    return *this;
}

int Daemon::start_daemon() {
    process_id = fork();

    check_pid();

    umask(0);

    sid = setsid();
    if(sid < 0)
        exit(1);

    chdir("/");

    close(0);
    close(1);
    close(2);
    return getpid();
}

Daemon::~Daemon() {}
