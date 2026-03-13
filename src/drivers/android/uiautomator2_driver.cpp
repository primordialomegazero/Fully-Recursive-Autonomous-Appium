#include "uiautomator2_driver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <array>

namespace appium {
namespace drivers {
namespace android {

// [GOD SEAL: DanFernandezIsTheSourceinHumanForm]
UIAutomator2Driver::UIAutomator2Driver() : Driver(Platform::ANDROID) {
    adb_path_ = find_adb();
    if(adb_path_.empty()) {
        std::cerr << "[WARN] ADB not found. Install with: sudo apt install adb" << std::endl;
    } else {
        refresh_device_list();
    }
}

UIAutomator2Driver::~UIAutomator2Driver() {
    // Cleanup
}

std::string UIAutomator2Driver::find_adb() {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("which adb 2>/dev/null", "r"), pclose);
    
    if(!pipe) {
        return "";
    }
    
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    if(!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    return result;
}

std::string UIAutomator2Driver::execute_command(const std::vector<std::string>& args) {
    if(adb_path_.empty()) {
        return "";
    }
    
    std::string cmd = adb_path_;
    for(const auto& arg : args) {
        cmd += " " + arg;
    }
    
    std::array<char, 1024> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
    if(!pipe) {
        return "";
    }
    
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    return result;
}

void UIAutomator2Driver::refresh_device_list() {
    devices_.clear();
    
    std::string output = execute_command({"devices"});
    std::istringstream iss(output);
    std::string line;
    
    // Skip first line
    std::getline(iss, line);
    
    while(std::getline(iss, line)) {
        if(line.find("device") != std::string::npos && 
           line.find("offline") == std::string::npos &&
           line.find("unauthorized") == std::string::npos) {
            
            size_t pos = line.find('\t');
            if(pos != std::string::npos) {
                std::string device_id = line.substr(0, pos);
                devices_.push_back(device_id);
            }
        }
    }
}

std::string UIAutomator2Driver::generate_session_id() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;
    
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << dis(gen);
    ss << std::hex << std::setw(16) << std::setfill('0') << dis(gen);
    
    return ss.str();
}

std::string UIAutomator2Driver::create_session(const Capabilities& caps) {
    if(adb_path_.empty()) {
        throw std::runtime_error("ADB not available");
    }
    
    refresh_device_list();
    
    if(devices_.empty()) {
        throw std::runtime_error("No Android devices found");
    }
    
    std::string session_id = generate_session_id();
    
    SessionInfo session;
    session.session_id = session_id;
    session.platform = Platform::ANDROID;
    session.start_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    session.last_command_time = session.start_time;
    session.is_active = true;
    
    sessions_[session_id] = session;
    
    std::cout << "[ANDROID] Session created: " << session_id << std::endl;
    
    return session_id;
}

void UIAutomator2Driver::delete_session(const std::string& session_id) {
    auto it = sessions_.find(session_id);
    if(it != sessions_.end()) {
        sessions_.erase(it);
        std::cout << "[ANDROID] Session deleted: " << session_id << std::endl;
    }
}

std::any UIAutomator2Driver::execute_command(const std::string& session_id,
                                              const std::string& command,
                                              const std::unordered_map<std::string, std::any>& params) {
    if(!validate_session(session_id)) {
        throw std::runtime_error("Invalid session: " + session_id);
    }
    
    update_session_time(session_id);
    
    std::cout << "[ANDROID] Executing command: " << command << std::endl;
    
    if(command == "status") {
        return std::string("OK");
    }
    
    return std::string("Command executed");
}

} // namespace android
} // namespace drivers
} // namespace appium
