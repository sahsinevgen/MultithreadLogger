#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <fstream>


struct ILogger {
    virtual void log(std::string str) = 0;
    virtual void stop() = 0;
};


class MyLogger: ILogger {
public:
    MyLogger(std::string filename);

    void log(std::string str) override;

    void stop() override;

private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::string> queue;
    std::string log_filename;
    std::thread writer;
    std::atomic<bool> is_stopped = false;
    std::atomic<int> logs_counter = 0;

    void write_loop();
};

#endif // MY_LOGGER_H