//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "logger.h"

#include <chrono>
#include <iomanip>
#include <sstream>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : logFile("log.txt", std::ios_base::trunc) {
    logThread = std::thread(&Logger::ProcessLogs, this);
}

Logger::~Logger() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        exitFlag = true;
    }
    queueCondition.notify_all();

    if (logThread.joinable()) {
        logThread.join();
    }
}

void Logger::Log(const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push_back(GetCurrentTimestamp() + " " + message);
    }
    queueCondition.notify_one();
}

void Logger::Flush() {
    std::unique_lock<std::mutex> lock(queueMutex);
    DrainQueue();
}

void Logger::ProcessLogs() {
    std::unique_lock<std::mutex> lock(queueMutex);

    while (true) {
        queueCondition.wait(lock, [this] { return !logQueue.empty() || exitFlag; });

        DrainQueue();

        if (exitFlag) {
            break;
        }
    }
}

void Logger::DrainQueue() {
    for (const std::string& entry : logQueue) {
        logFile << entry << std::endl;
    }
    logQueue.clear();
    logFile.flush();
}

std::string Logger::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
