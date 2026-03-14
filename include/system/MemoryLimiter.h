#ifndef MEMORY_LIMITER_H
#define MEMORY_LIMITER_H

#include "SystemDetector.h"
#include <mutex>
#include <atomic>

class MemoryLimiter {
public:
    static MemoryLimiter& GetInstance();
    
    void SetMaxMemoryPercent(int percent);
    int GetMaxMemoryPercent() const { return max_percent_; }
    
    long long GetMaxMemoryBytes() const;
    long long GetCurrentMemoryUsage() const;
    bool CanAllocate(size_t bytes) const;
    
    void UpdateFromSystem(const SystemSpecs& specs);
    int GetOptimalThreadCount(int requested) const;
    
private:
    MemoryLimiter();
    ~MemoryLimiter() = default;
    
    std::atomic<int> max_percent_;
    std::atomic<long long> total_system_ram_;
    std::atomic<long long> max_allowed_ram_;
    mutable std::mutex mutex_;
};

#endif
