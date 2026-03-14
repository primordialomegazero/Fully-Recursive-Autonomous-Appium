#include "../../include/monitoring/MetricsServer.h"
#include "../../include/system/MemoryLimiter.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <chrono>

namespace monitoring {

MetricsServer::MetricsServer(int port) 
    : port_(port), server_socket_(-1), running_(false), start_time_(0),
      total_requests_(0), error_count_(0), active_sessions_(0) {
    
    current_.uptime_seconds = 0;
    current_.cpu_usage_percent = 0;
    current_.memory_usage_mb = 0;
    current_.memory_limit_mb = 0;
    current_.thread_count = 0;
    current_.active_sessions = 0;
    current_.total_requests = 0;
    current_.error_count = 0;
}

MetricsServer::~MetricsServer() {
    Stop();
}

bool MetricsServer::Start() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) return false;
    
    int opt = 1;
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);
    
    if (bind(server_socket_, (struct sockaddr*)&addr, sizeof(addr)) < 0) return false;
    if (listen(server_socket_, 10) < 0) return false;
    
    running_ = true;
    start_time_ = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    
    http_thread_ = std::thread(&MetricsServer::AcceptLoop, this);
    
    std::cout << "Metrics server started on port " << port_ << std::endl;
    return true;
}

void MetricsServer::Stop() {
    running_ = false;
    if (http_thread_.joinable()) {
        http_thread_.join();
    }
    if (server_socket_ >= 0) {
        close(server_socket_);
        server_socket_ = -1;
    }
}

void MetricsServer::AcceptLoop() {
    while (running_) {
        int client = accept(server_socket_, nullptr, nullptr);
        if (client >= 0) {
            HandleRequest(client);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void MetricsServer::HandleRequest(int client_socket) {
    char buffer[4096] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    
    std::string request(buffer);
    std::string response;
    
    UpdateSystemMetrics();
    
    if (request.find("GET /metrics") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "\r\n";
        response += GetPrometheusMetrics();
    } else if (request.find("GET /health") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: application/json\r\n";
        response += "\r\n";
        response += "{\"status\":\"healthy\",\"uptime\":" + std::to_string(current_.uptime_seconds) + "}";
    } else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    
    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
}

void MetricsServer::UpdateSystemMetrics() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    
    int64_t now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
    current_.uptime_seconds = now - start_time_;
    
    // Memory usage
    long long mem_usage = MemoryLimiter::GetInstance().GetCurrentMemoryUsage();
    current_.memory_usage_mb = mem_usage / (1024 * 1024);
    current_.memory_limit_mb = MemoryLimiter::GetInstance().GetMaxMemoryBytes() / (1024 * 1024);
    
    // Thread count
    current_.thread_count = std::thread::hardware_concurrency();
    
    // Request stats
    current_.total_requests = total_requests_;
    current_.error_count = error_count_;
    current_.active_sessions = active_sessions_;
}

void MetricsServer::RecordRequest() {
    total_requests_++;
}

void MetricsServer::RecordError() {
    error_count_++;
}

void MetricsServer::SetActiveSessions(int count) {
    active_sessions_ = count;
}

SystemMetrics MetricsServer::GetMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return current_;
}

std::string MetricsServer::GetPrometheusMetrics() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    std::stringstream ss;
    
    ss << "# HELP appium_uptime_seconds Uptime in seconds\n";
    ss << "# TYPE appium_uptime_seconds counter\n";
    ss << "appium_uptime_seconds " << current_.uptime_seconds << "\n\n";
    
    ss << "# HELP appium_memory_usage_mb Memory usage in MB\n";
    ss << "# TYPE appium_memory_usage_mb gauge\n";
    ss << "appium_memory_usage_mb " << current_.memory_usage_mb << "\n\n";
    
    ss << "# HELP appium_memory_limit_mb Memory limit in MB (25% of system RAM)\n";
    ss << "# TYPE appium_memory_limit_mb gauge\n";
    ss << "appium_memory_limit_mb " << current_.memory_limit_mb << "\n\n";
    
    ss << "# HELP appium_active_sessions Active sessions\n";
    ss << "# TYPE appium_active_sessions gauge\n";
    ss << "appium_active_sessions " << current_.active_sessions << "\n\n";
    
    ss << "# HELP appium_requests_total Total requests\n";
    ss << "# TYPE appium_requests_total counter\n";
    ss << "appium_requests_total " << current_.total_requests << "\n\n";
    
    ss << "# HELP appium_errors_total Total errors\n";
    ss << "# TYPE appium_errors_total counter\n";
    ss << "appium_errors_total " << current_.error_count << "\n\n";
    
    return ss.str();
}

} // namespace monitoring
