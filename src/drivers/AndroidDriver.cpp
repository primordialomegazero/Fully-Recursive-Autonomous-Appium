#include "../../include/drivers/AndroidDriver.h"
#include <cstdlib>
#include <sstream>

namespace android {
AndroidDriver::AndroidDriver() {}
AndroidDriver::~AndroidDriver() {}

std::vector<DeviceInfo> AndroidDriver::GetDevices() {
    std::vector<DeviceInfo> devices;
    FILE* pipe = popen("adb devices 2>/dev/null", "r");
    if (!pipe) return devices;
    
    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;
    pclose(pipe);
    
    std::istringstream iss(result);
    std::string line;
    std::getline(iss, line); // Skip header
    
    while (std::getline(iss, line)) {
        if (line.find("device") != std::string::npos && line.find("offline") == std::string::npos) {
            DeviceInfo info;
            info.udid = line.substr(0, line.find('\t'));
            info.online = true;
            devices.push_back(info);
        }
    }
    return devices;
}

std::string AndroidDriver::CreateSession(const std::string& udid, const std::map<std::string, std::string>& caps) {
    return "session_" + udid;
}
} // namespace android
