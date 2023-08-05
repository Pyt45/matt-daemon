#include "../include/Tintin_reporter.hpp"

Tintin_reporter* Tintin_reporter::logger = nullptr;

Tintin_reporter::Tintin_reporter() {}

Tintin_reporter& Tintin_reporter::getLogger() {
    if (!logger)
        logger = new Tintin_reporter();
    return *logger;
}

std::string Tintin_reporter::getLevel(t_level level) {
    return (level == LOG ? "LOG" : level == INFO ? "INFO" : "ERROR");
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter& src) {
    *this = src;
}

Tintin_reporter& Tintin_reporter::operator=(const Tintin_reporter& src) {
    if (this != &src) {
    }
    return *this;
}

std::string Tintin_reporter::getMessagePrefix(t_tag tag, std::string msg) {
    std::string prefix;
    if (tag == STARTED)
        prefix = "Started.";
    else if (tag == CREATE_SERVER)
        prefix = "Creating server.";
    else if (tag == SERVER)
        prefix = "Server created.";
    else if (tag == DAEMON_MODE)
        prefix = "Entering Daemon mode.";
    else if (tag == PID)
        prefix = "started. PID: " + msg + ".";
    else if (tag == SIGNAL)
        prefix = "Signal handler.";
    else if (tag == ERR_LOCKFILE)
        prefix = "Error file locked.";
    else if (tag == ERR_UNLOCKFILE)
        prefix = "Error file unlocked.";
    else if (tag == USER_INPUT)
        prefix = "User input: " + msg;
    else if (tag == REQUEST_QUIT)
        prefix = "Request quit.";
    else if (tag == QUIT)
        prefix = "Quitting.";
    else if (tag == ERR_SERVER)
        prefix = "Error server: " + msg;
    return prefix + "\n";
}

std::string Tintin_reporter::getTimeStamp() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       timestamp[80];
    tstruct = *localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%d/%m/%Y-%X", &tstruct);
    return timestamp;
}

void Tintin_reporter::log(t_tag tag, t_level level, std::string message) {

    const std::filesystem::path fs_path{LOG_DIR};
    if (std::filesystem::exists(fs_path) == false)
        std::filesystem::create_directory(fs_path);
    std::ofstream logfile(LOG_FILE, std::ios_base::app);
    if (logfile.is_open())
    {
        logfile << format(getTimeStamp(), getLevel(level), getMessagePrefix(tag, message));
        logfile.close();
    }
}

Tintin_reporter::~Tintin_reporter() {
    if (logger)
        delete logger;
}
