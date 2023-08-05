#include "../include/Matt_daemon.hpp"
#include "../include/server.hpp"
// #include "../include/Lock_file.hpp"
// #include "../include/Tintin_reporter.hpp"

Tintin_reporter& logger = Tintin_reporter::getLogger();
bool terminate = false;

int sig_arr[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGIO, SIGSYS };
// int sig_arr[] = { SIGTERM, SIGINT, SIGQUIT, SIGKILL, SIGHUP };
void signal_handler(int sig) {
    std::cout << "terminated\n";
    logger.log(SIGNAL, INFO, "");
    terminate = true;
}


// fd = open("/Users/ayoubaqlzim/matt_daemon.lock", O_RDWR | O_CREAT, 0600);
// if (fd < 0) {
//     std::cout << "error creating matt_daemon file\n";
//     exit(1);
// }
// if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
//     if (errno == EWOULDBLOCK) {
//         log_message("matt_daemon.lock Already locked\n");
//     }
//     close(fd);
//     log_message("Can't open :/var/lock/matt_daemon.lock\n");
//     exit(1);
// }
// log_message("[INFO] started\n");

// signal(SIGKILL, signal_handler);
// if (terminated == true) {
//     if (flock(fd, LOCK_UN) == -1) {
//     }
//     close(fd);
//     std::remove("/Users/ayoubaqlzim/matt_daemon.lock");
//     exit(1);
// }

int main() {
    Daemon daemon;
    for (int i = 0; i < sizeof(sig_arr)/sizeof(sig_arr[0]); i++)
        signal(sig_arr[i], signal_handler);
    Lock_file locker = Lock_file::getInstance();
    int fd_lock = locker.lock_file();
    if (fd_lock < 0) {
        // terminate = true;
        std::cerr << "Can't open: /var/lock/matt_daemon.lock\n";
        locker.realease_file();
        logger.log(QUIT, INFO, "");
        exit(1);
    }
    logger.log(STARTED, INFO, "");
    logger.log(CREATE_SERVER, INFO, "");
    logger.log(SERVER, INFO, "");


    int p = daemon.start_daemon();
    logger.log(DAEMON_MODE, INFO, "");
    logger.log(PID, INFO, std::to_string(p));
    logger.log(STARTED, INFO, "");
    
    Server s(PORT);
    s.run(&terminate);

    locker.realease_file();
    logger.log(QUIT, INFO, "");
}
