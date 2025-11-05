#include "easy-log.h"
#include <thread>
#include <chrono>

void do_work_in_b() {
    log_info("Module B: Hello from B");
    set_log_file("logs/module_b_custom.log");
    log_warning("Module B: Using custom log");

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    log_error("Module B: Simulated error");

    // Empty filename test
    set_log_file("");
    log_info("Module B: Logging after empty filename (should fail)");

    set_log_file("logs/shared_final.log");
    log_info("Module B: Also using shared_final.log now");
}