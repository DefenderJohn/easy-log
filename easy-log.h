#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace simple_logger {

class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    void set_log_file(const std::string& filename) {
        if (filename.empty()) {
            std::cerr << "Warning: empty log filename ignored." << std::endl;
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        std::ofstream temp_file;
        temp_file.open(filename, std::ios::app);
        if (!temp_file.is_open()) {
            std::cerr << "Cannot open log file: " << filename << std::endl;
            return;
        }

        log_file_.close();
        log_file_ = std::move(temp_file);
        current_filename_ = filename;
    }

    void write(const std::string& level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        ensure_open();
        if (!log_file_.is_open()) {
            std::cerr << "[FALLBACK][" << level << "] " << message << std::endl;
            return;
        }

        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        log_file_ << std::put_time(&tm, "[%Y-%m-%d %H:%M:%S] ")
                  << "[" << level << "] "
                  << message << std::endl;
    }

private:
    Logger() {
        set_log_file("app.log");
    }

    void ensure_open() {
        if (!log_file_.is_open()) {
            set_log_file(current_filename_);
        }
    }

    std::string current_filename_ = "app.log";
    std::ofstream log_file_;
    mutable std::mutex mutex_;
};

inline void log_info(const std::string& message) {
    Logger::instance().write("INFO", message);
}

inline void log_warning(const std::string& message) {
    Logger::instance().write("WARNING", message);
}

inline void log_error(const std::string& message) {
    Logger::instance().write("ERROR", message);
}

inline void set_log_file(const std::string& filename) {
    Logger::instance().set_log_file(filename);
}

} // namespace simple_logger

using simple_logger::log_info;
using simple_logger::log_warning;
using simple_logger::log_error;
using simple_logger::set_log_file;