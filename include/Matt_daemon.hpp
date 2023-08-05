#pragma once

#include "Lock_file.hpp"


class Daemon {
    private:
        pid_t process_id;
        pid_t sid;
    public:
        Daemon();
        Daemon(const Daemon& src);
        Daemon& operator=(const Daemon& src);
        int start_daemon();
        void check_pid();
        ~Daemon();
};
