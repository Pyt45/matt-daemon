#include "../include/Lock_file.hpp"

Lock_file* Lock_file::instance = nullptr;

Lock_file::Lock_file(): logger(Tintin_reporter::getLogger()) {}

Lock_file& Lock_file::getInstance() {
    if (!instance)
        instance = new Lock_file();
    return *instance;
}

Lock_file::Lock_file(const Lock_file& src): logger(src.logger) {
    *this = src;
}

Lock_file& Lock_file::operator=(const Lock_file& src) {
    if (this != &src) {
        this->lock_fd = src.lock_fd;
    }
    return *this;
}

int Lock_file::lock_file() {
    lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0644);
    if (lock_fd < 0) {
        logger.log(ERR_OPENFILE, ERROR, "");
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
    if (std::filesystem::exists(filepath))
        std::filesystem::remove(filepath);
}

Lock_file::~Lock_file() {
    if (instance)
        delete instance;
}