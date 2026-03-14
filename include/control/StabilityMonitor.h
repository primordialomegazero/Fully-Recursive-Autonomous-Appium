#ifndef STABILITY_MONITOR_H
#define STABILITY_MONITOR_H

namespace control {
class StabilityMonitor {
public:
    StabilityMonitor();
    ~StabilityMonitor();
    void Start();
    void Stop();
};
} // namespace control
#endif
