#include "easy-log.h"
#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>

// Declare functions from other modules
void do_work_in_a();
void do_work_in_b();

int main() {
    std::cout << "=== Starting Comprehensive Logger Test ===\n";

    // Test 1: Basic logging to default file
    log_info("Main: Default app.log entry");
    log_warning("Main: Warning in default log");
    log_error("Main: Error in default log");

    // Test 2: Change to valid new file
    set_log_file("logs/main_custom.log");
    log_info("Main: Now writing to main_custom.log");

    // Test 3: Multiple threads + modules
    std::vector<std::thread> threads;
    threads.emplace_back(do_work_in_a);
    threads.emplace_back(do_work_in_b);

    // Main thread also logs concurrently
    for (int i = 0; i < 5; ++i) {
        log_info("Main thread concurrent log " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Test 4: Final switch
    set_log_file("logs/final_summary.log");
    log_info("Main: All modules finished. Final log here.");

    // Test 5: Verify singleton address (optional sanity)
    auto& inst1 = simple_logger::Logger::instance();
    auto& inst2 = simple_logger::Logger::instance();
    if (&inst1 == &inst2) {
        log_info("Singleton integrity confirmed.");
    }

    std::cout << "=== Test completed. Check log files in current directory ===\n";

    // Optional: list generated files
    std::cout << "Generated files:\n";
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.path().extension() == ".log") {
            std::cout << "  - " << entry.path().filename().string() << "\n";
        }
    }

    return 0;
}