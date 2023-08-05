#include "../include/Matt_daemon.hpp"

Daemon::Daemon() {}

void Daemon::check_pid() {
    if (process_id < 0) {
        std::cout << "failed to fork\n";
        exit(1);
    }
    if (process_id > 0)
    {
        std::cout << "process_id of child process " << process_id << std::endl;
        exit(0);
    }
}

int Daemon::start_daemon() {
    if (geteuid()) {
        std::cerr << "Error need root priviliges\n";
        exit(1);
    }
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
