#pragma once

#include "Core.hpp"

typedef enum s_level {
    LOG,
    INFO,
    ERROR,
}           t_level;

typedef enum s_tag {
    STARTED,
    CREATE_SERVER,
    SERVER,
    DAEMON_MODE,
    PID,
    SIGNAL,
    ERR_LOCKFILE,
    ERR_UNLOCKFILE,
    USER_INPUT,
    QUIT,
    REQUEST_QUIT,
    ERR_SERVER,
    ERR_OPENFILE, 
}           t_tag;
 
class Tintin_reporter {
    private:
        Tintin_reporter();
        static Tintin_reporter* logger;
    public:
        std::string getLevel(t_level level);
        std::string getMessagePrefix(t_tag tag, std::string msg);
        std::string getTimeStamp();
        Tintin_reporter(const Tintin_reporter& src);
        Tintin_reporter& operator=(const Tintin_reporter& src);
        ~Tintin_reporter();
        static Tintin_reporter& getLogger();
        void log(t_tag tag, t_level level, std::string message);
};
