#pragma once

#include "Lock_file.hpp"


class Daemon {
    private:
        pid_t process_id;
        pid_t sid;
        int fd;
    public:
        Daemon();
        int start_daemon();
        void check_pid();
        ~Daemon();
};
