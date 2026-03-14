#ifndef IOS_DRIVER_H
#define IOS_DRIVER_H

#include <string>
#include <vector>

namespace ios {
struct DeviceInfo {
    std::string udid;
    std::string name;
    bool simulator;
    bool online;
};
class IOSDriver {
public:
    IOSDriver();
    ~IOSDriver();
    std::vector<DeviceInfo> GetDevices();
};
} // namespace ios
#endif
