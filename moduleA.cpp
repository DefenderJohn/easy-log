#include "easy-log.h"
#include <thread>
#include <chrono>

void do_work_in_a() {
    log_info("Module A: Starting work");
    set_log_file("logs/module_a_stage1.log");
    log_warning("Module A: Switched to stage1 log");

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    log_info("Module A: Mid work");

    // Try an invalid path (may fail)
    set_log_file("/root/protected.log");  // Usually not writable
    log_error("Module A: This should fallback to stderr or fail gracefully");

    set_log_file("logs/shared_final.log");
    log_info("Module A: Back to shared log");
}