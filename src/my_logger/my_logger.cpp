#include "my_logger.h"

MyLogger::MyLogger(std::string filename): log_filename(filename) {
    writer = std::thread(&MyLogger::write_loop, this);
}

void MyLogger::log(std::string str) {
    if (is_stopped) {
        return;
    }

    {
        std::unique_lock lock(m);

        // if (is_stopped) {
        //     return;
        // }

        queue.push(str);
    }

    cv.notify_one();
}

void MyLogger::stop() {
    {
        std::unique_lock lock(m);
        is_stopped = true;
    }
    cv.notify_one();
    writer.join();
}

void MyLogger::write_loop() {
    std::ofstream log_file;
    log_file.open(log_filename);

    while (1) {
        std::string log_message;
        {
            std::unique_lock lock(m);
            if (!is_stopped) {
                cv.wait(lock, 
                    [this] {
                        return !queue.empty() || is_stopped;
                    });
            }
            
            if (queue.empty() && is_stopped) {
                break;
            }

            log_message = queue.front();
            queue.pop();
        }

        log_file << log_message;
    }

    log_file.close();
}