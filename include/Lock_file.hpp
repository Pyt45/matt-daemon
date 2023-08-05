#pragma once

#include "Tintin_reporter.hpp"

class Lock_file {
    private:
        int lock_fd;
        Lock_file();
        static Lock_file* instance;
        Tintin_reporter& logger;
    public:
        ~Lock_file();
        int lock_file();
        void realease_file();
        static Lock_file& getInstance();
};
