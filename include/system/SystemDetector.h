#ifndef SYSTEM_DETECTOR_H
#define SYSTEM_DETECTOR_H

#include <string>

struct SystemSpecs {
    int cpu_cores;
    std::string cpu_model;
    double cpu_freq_ghz;
    long long total_ram_mb;
    long long available_ram_mb;
    
    enum Tier { TIER_LOW, TIER_MEDIUM, TIER_HIGH, TIER_ULTRA } tier;
    
    int optimal_thread_pool_size;
    int optimal_sleep_ms;
};

class SystemDetector {
public:
    static SystemSpecs Detect();
};

#endif
