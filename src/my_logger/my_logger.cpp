#include "my_logger.h"

MyLogger::MyLogger(std::string filename): log_filename(filename) {
    writer = std::thread(&MyLogger::write_loop, this);
}

void MyLogger::log(std::string str) {
    logs_counter++;

    // if at this moment stop() is called log will not save

    if (is_stopped) {
        logs_counter--;
        return;
    }

    {
        std::unique_lock lock(m);

        queue.push(str);
    }

    cv.notify_one();
}

void MyLogger::stop() {
    is_stopped = true;

    cv.notify_one();
    writer.join();
}

void MyLogger::write_loop() {
    std::ofstream log_file;
    log_file.open(log_filename);

    while (1) {
        std::vector<std::string> logs_to_process;
        {
            std::unique_lock lock(m);
            if (!is_stopped) {
                cv.wait(lock, 
                    [this] {
                        return !queue.empty() || is_stopped;
                    });
            }
            
            if (is_stopped && logs_counter == 0) {
                break;
            }
            
            logs_to_process.reserve(queue.size());

            while (!queue.empty()) {
                logs_to_process.push_back(std::move(queue.front()));
                queue.pop();
            }
        }
        for (const auto& log: logs_to_process) {
            logs_counter--;
            log_file << log;
        }
    }

    log_file.close();
}