#include "../include/Matt_daemon.hpp"
#include "../include/server.hpp"


Tintin_reporter& logger = Tintin_reporter::getLogger();
bool terminate = false;

int sig_arr[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGIO, SIGSYS };

void signal_handler(int sig) {
    logger.log(SIGNAL, INFO, "");
    terminate = true;
}



int main() {
    Daemon daemon;
    if (geteuid()) {
        std::cerr << "Error need root priviliges\n";
        exit(1);
    }
    for (int i = 0; i < sizeof(sig_arr)/sizeof(sig_arr[0]); i++)
        signal(sig_arr[i], signal_handler);
    Lock_file locker = Lock_file::getInstance();
    int fd_lock = locker.lock_file();
    if (fd_lock < 0) {
        std::cerr << "Can't open: /var/lock/matt_daemon.lock\n";
        locker.realease_file();
        logger.log(QUIT, INFO, "");
        exit(1);
    }
    logger.log(STARTED, INFO, "");
    logger.log(CREATE_SERVER, INFO, "");
    Server s(PORT);
    logger.log(SERVER, INFO, "");


    int p = daemon.start_daemon();
    logger.log(DAEMON_MODE, INFO, "");
    logger.log(PID, INFO, std::to_string(p));
    logger.log(STARTED, INFO, "");
    
    s.run(&terminate);

    locker.realease_file();
    logger.log(QUIT, INFO, "");
    close(fd_lock);
}
