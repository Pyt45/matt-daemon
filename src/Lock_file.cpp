#include "../include/Lock_file.hpp"

Lock_file* Lock_file::instance = nullptr;

Lock_file::Lock_file(): logger(Tintin_reporter::getLogger()) {}

Lock_file& Lock_file::getInstance() {
    if (!instance)
        instance = new Lock_file();
    return *instance;
}

int Lock_file::lock_file() {
    lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
    if (lock_fd < 0) {
        logger.log(ERR_LOCKFILE, ERROR, "");
        return -1;
    }
    if (flock(lock_fd, LOCK_EX | LOCK_NB) == -1) {
        logger.log(ERR_LOCKFILE, ERROR, "");
        return -1;
    }
    return lock_fd;
}

void Lock_file::realease_file() {
    if (flock(lock_fd, LOCK_UN) == -1) {
        logger.log(ERR_UNLOCKFILE, ERROR, "");
    }
    std::filesystem::path filepath{LOCK_FILE};

    std::filesystem::remove(filepath);
}

Lock_file::~Lock_file() {
    if (instance)
        delete instance;
}