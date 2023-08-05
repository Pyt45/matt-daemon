#include "../include/Lock_file.hpp"

Lock_file* Lock_file::instance = nullptr;

Lock_file::Lock_file(): logger(Tintin_reporter::getLogger()) {}

Lock_file& Lock_file::getInstance() {
    if (!instance)
        instance = new Lock_file();
    return *instance;
}

int Lock_file::lock_file() {
    lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0600);
    if (lock_fd < 0) {
        logger.log(ERR_LOCKFILE, ERROR, "");
        return -1;
    }
    if (flock(lock_fd, LOCK_EX | LOCK_NB) == -1)
        logger.log(ERR_LOCKFILE, ERROR, "");
    return lock_fd;
}

void Lock_file::realease_file() {
    if (flock(lock_fd, LOCK_UN) == -1) {
        logger.log(ERR_UNLOCKFILE, ERROR, "");
    }
}

Lock_file::~Lock_file() {
    if (instance)
        delete instance;
}