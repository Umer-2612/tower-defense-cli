//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Message format is "2024-08-30 14:48:16 Starting game..."

class Logger {
   public:
    static Logger& GetInstance();

    ~Logger();

    void Log(const std::string& message);
    void Flush();

   private:
    Logger();

    std::ofstream logFile;
    std::thread logThread;
    std::vector<std::string> logQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    bool exitFlag = false;

    void ProcessLogs();
    void DrainQueue();
    std::string GetCurrentTimestamp();
};

#endif  // LOGGER_H
