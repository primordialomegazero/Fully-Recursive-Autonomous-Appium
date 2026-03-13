#ifndef DRIVER_INTERFACE_H
#define DRIVER_INTERFACE_H

#include "../../include/god_watermark.h"
#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <chrono>

namespace appium {
namespace drivers {

enum class Platform {
    ANDROID,
    IOS,
    WINDOWS,
    MAC,
    LINUX
};

struct Capabilities {
    std::string platform_name;
    std::string platform_version;
    std::string device_name;
    std::string app;
    std::string automation_name;
    bool no_reset;
    bool full_reset;
    std::unordered_map<std::string, std::any> extra;
};

struct SessionInfo {
    std::string session_id;
    Platform platform;
    uint64_t start_time;
    uint64_t last_command_time;
    bool is_active;
    std::unordered_map<std::string, std::any> capabilities;
};

class Driver {
protected:
    std::unordered_map<std::string, SessionInfo> sessions_;
    Platform platform_;
    
    static constexpr double PHI = 1.6180339887498948482;
    
public:
    Driver(Platform platform) : platform_(platform) {}
    virtual ~Driver() {}
    
    virtual std::string create_session(const Capabilities& caps) = 0;
    virtual void delete_session(const std::string& session_id) = 0;
    virtual std::any execute_command(const std::string& session_id, 
                                      const std::string& command,
                                      const std::unordered_map<std::string, std::any>& params) = 0;
    
    void cleanup_stale_sessions() {
        auto now = std::chrono::system_clock::now();
        auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
        
        for(auto it = sessions_.begin(); it != sessions_.end();) {
            if(now_sec - it->second.last_command_time > 60) {
                it = sessions_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    Platform get_platform() const { return platform_; }
    size_t active_session_count() const { return sessions_.size(); }
    
protected:
    bool validate_session(const std::string& id) {
        return sessions_.find(id) != sessions_.end();
    }
    
    void update_session_time(const std::string& id) {
        auto it = sessions_.find(id);
        if(it != sessions_.end()) {
            auto now = std::chrono::system_clock::now();
            it->second.last_command_time = std::chrono::duration_cast<std::chrono::seconds>(
                now.time_since_epoch()).count();
        }
    }
};

} // namespace drivers
} // namespace appium

#endif
