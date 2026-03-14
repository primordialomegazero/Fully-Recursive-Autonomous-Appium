#include "../../include/system/SystemDetector.h"
#include <fstream>
#include <thread>
#include <unistd.h>
#include <sys/sysinfo.h>

SystemSpecs SystemDetector::Detect() {
    SystemSpecs specs;
    
    // Get CPU cores
    specs.cpu_cores = std::thread::hardware_concurrency();
    if (specs.cpu_cores == 0) specs.cpu_cores = 2;
    
    // Get CPU model
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                specs.cpu_model = line.substr(colon + 2);
                break;
            }
        }
    }
    if (specs.cpu_model.empty()) specs.cpu_model = "Unknown CPU";
    
    // Get CPU frequency
    specs.cpu_freq_ghz = 2.0;
    std::ifstream cpuinfo_max("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (cpuinfo_max.is_open()) {
        int khz;
        cpuinfo_max >> khz;
        specs.cpu_freq_ghz = khz / 1000000.0;
    }
    
    // Get RAM info
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        specs.total_ram_mb = info.totalram * info.mem_unit / (1024 * 1024);
        specs.available_ram_mb = info.freeram * info.mem_unit / (1024 * 1024);
    } else {
        specs.total_ram_mb = 4096;
        specs.available_ram_mb = 1024;
    }
    
    // Determine tier and optimal settings
    if (specs.cpu_cores >= 8 && specs.total_ram_mb >= 16000) {
        specs.tier = SystemSpecs::TIER_ULTRA;
        specs.optimal_thread_pool_size = specs.cpu_cores * 2;
        specs.optimal_sleep_ms = 1;
    } else if (specs.cpu_cores >= 4 && specs.total_ram_mb >= 8000) {
        specs.tier = SystemSpecs::TIER_HIGH;
        specs.optimal_thread_pool_size = specs.cpu_cores;
        specs.optimal_sleep_ms = 5;
    } else if (specs.cpu_cores >= 2 && specs.total_ram_mb >= 4000) {
        specs.tier = SystemSpecs::TIER_MEDIUM;
        specs.optimal_thread_pool_size = specs.cpu_cores;
        specs.optimal_sleep_ms = 10;
    } else {
        specs.tier = SystemSpecs::TIER_LOW;
        specs.optimal_thread_pool_size = 1;
        specs.optimal_sleep_ms = 20;
    }
    
    return specs;
}
