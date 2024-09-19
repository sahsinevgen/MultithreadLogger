#include <thread>
#include <string>
#include <sstream>

#include "my_logger/my_logger.cpp"

void generate_logs(MyLogger &logger, std::string prefix) {
    for (int i = 0; i < 5; i++) {
        std::stringstream msg;
        msg << prefix << "message №" << std::to_string(i) << "\n";
        logger.log(msg.str());
    }
};

int main() {
    MyLogger logger("log.txt");

    std::thread threads[5];
    for (int i = 0; i < 5; i++) {
        std::stringstream prefix;
        prefix << "thread №" << i << ": ";
        threads[i] = std::thread(generate_logs, std::ref(logger), prefix.str());
    }

    for (int i = 0; i < 5; i++) {
        threads[i].join();
    }

    logger.stop();

    std::thread bad_thread = std::thread(generate_logs, std::ref(logger), "bad thread: ");

    bad_thread.join();
}