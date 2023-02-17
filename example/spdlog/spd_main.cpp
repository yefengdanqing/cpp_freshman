#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging

int main(int argc, char* argv[]) {
    std::cout << "start spdlog" << std::endl;
    try {
        // create a file rotating logger with 5mb size max and 3 rotated files
        auto file_logger = spdlog::rotating_logger_mt("file_logger", "./log/myfilename", 1024 * 1024 * 1, 3);

        for (int i = 0; i < 1024 * 1024 * 5 * 4; i++)
            file_logger->info("This is a rotating logger: {}", i);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    } 
}