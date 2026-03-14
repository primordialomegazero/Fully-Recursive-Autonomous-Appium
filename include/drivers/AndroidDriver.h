#ifndef ANDROID_DRIVER_H
#define ANDROID_DRIVER_H

#include <string>
#include <vector>
#include <map>

namespace android {
struct DeviceInfo {
    std::string udid;
    std::string model;
    std::string android_version;
    bool online;
    bool emulator;
};
class AndroidDriver {
public:
    AndroidDriver();
    ~AndroidDriver();
    std::vector<DeviceInfo> GetDevices();
    std::string CreateSession(const std::string& udid, const std::map<std::string, std::string>& caps);
};
} // namespace android
#endif
