#pragma once

#include <sys/file.h>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <time.h>
#include <stdexcept>

# define PORT 4242

#define format(date, level, message) "[" + date + "]" + " [ " + level + " ] - Matt_daemon: " + message

#ifdef __aarch64__
    #define LOCK_FILE "/Users/ayoubaqlzim/matt_daemon.lock"
    #define LOG_DIR "/Users/ayoubaqlzim/matt_daemon"
    #define LOG_FILE "/Users/ayoubaqlzim/matt_daemon/matt_daemon.log"
#else
    #define LOCK_FILE "/var/lock/matt_daemon.lock"
    #define LOG_DIR "/var/log/matt_daemon"
    #define LOG_FILE "/var/log/matt_daemon/matt_daemon.log"
#endif
