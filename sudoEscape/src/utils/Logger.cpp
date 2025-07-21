#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger() {
    logFile.open("sudoEscape.log", std::ios::app);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char buf[26];
#ifdef _WIN32
    ctime_s(buf, sizeof(buf), &now_c);
#else
    ctime_r(&now_c, buf);
#endif
    buf[24] = ' ';

    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << "[" << buf << "] " << message << std::endl;
    }
}
