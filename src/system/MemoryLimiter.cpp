#include "../../include/system/MemoryLimiter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/sysinfo.h>

MemoryLimiter::MemoryLimiter() : max_percent_(25), total_system_ram_(0), max_allowed_ram_(0) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        total_system_ram_ = info.totalram * info.mem_unit;
        max_allowed_ram_ = (total_system_ram_ * max_percent_) / 100;
    }
}

MemoryLimiter& MemoryLimiter::GetInstance() {
    static MemoryLimiter instance;
    return instance;
}

void MemoryLimiter::SetMaxMemoryPercent(int percent) {
    if (percent < 1) percent = 1;
    if (percent > 100) percent = 100;
    max_percent_ = percent;
    
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        total_system_ram_ = info.totalram * info.mem_unit;
        max_allowed_ram_ = (total_system_ram_ * max_percent_) / 100;
    }
}

long long MemoryLimiter::GetMaxMemoryBytes() const {
    return max_allowed_ram_;
}

long long MemoryLimiter::GetCurrentMemoryUsage() const {
    std::ifstream statm("/proc/self/statm");
    if (!statm.is_open()) return 0;
    
    long size, resident, share, text, lib, data, dt;
    statm >> size >> resident >> share >> text >> lib >> data >> dt;
    statm.close();
    
    // Convert pages to bytes (page size is usually 4096)
    long page_size = sysconf(_SC_PAGESIZE);
    return resident * page_size;
}

bool MemoryLimiter::CanAllocate(size_t bytes) const {
    long long current = GetCurrentMemoryUsage();
    return (current + bytes) <= max_allowed_ram_;
}

void MemoryLimiter::UpdateFromSystem(const SystemSpecs& specs) {
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        total_system_ram_ = info.totalram * info.mem_unit;
        max_allowed_ram_ = (total_system_ram_ * max_percent_) / 100;
    }
    
    std::cout << "Memory Limiter: " << max_percent_ << "% of system RAM" << std::endl;
    std::cout << "  Total RAM: " << total_system_ram_ / (1024*1024) << " MB" << std::endl;
    std::cout << "  Max allowed: " << max_allowed_ram_ / (1024*1024) << " MB" << std::endl;
}

int MemoryLimiter::GetOptimalThreadCount(int requested) const {
    long long current = GetCurrentMemoryUsage();
    double usage_ratio = static_cast<double>(current) / max_allowed_ram_;
    
    // Reduce threads if memory usage is high
    if (usage_ratio > 0.8) {
        return std::max(1, requested / 2);
    } else if (usage_ratio > 0.6) {
        return requested * 3 / 4;
    }
    return requested;
}
