/*
 * Fully Recursive Autonomous Appium
 * AdaptiveServer Implementation
 * 
 * THE SOURCE: DanFernandezIsTheSourceinHumanForm
 */

#include "../../include/core/AdaptiveServer.h"
#include "../../include/god/watermark.h"
#include "../../include/system/MemoryLimiter.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <fcntl.h>
#include <errno.h>
#include <chrono>

AdaptiveServer::AdaptiveServer(int port) 
    : port_(port), server_socket_(-1), running_(false) {
    specs_ = SystemDetector::Detect();
    
    // Apply memory limiter
    MemoryLimiter::GetInstance().SetMaxMemoryPercent(25); // 25% max RAM
    MemoryLimiter::GetInstance().UpdateFromSystem(specs_);
}

AdaptiveServer::~AdaptiveServer() {
    Stop();
}

bool AdaptiveServer::Start() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) return false;
    
    int opt = 1;
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);
    
    if (bind(server_socket_, (struct sockaddr*)&addr, sizeof(addr)) < 0) return false;
    if (listen(server_socket_, 100) < 0) return false;
    
    int flags = fcntl(server_socket_, F_GETFL, 0);
    fcntl(server_socket_, F_SETFL, flags | O_NONBLOCK);
    
    running_ = true;
    
    // Start thread pool with memory-aware sizing
    int thread_count = MemoryLimiter::GetInstance().GetOptimalThreadCount(specs_.optimal_thread_pool_size);
    for (int i = 0; i < thread_count; ++i)
        workers_.emplace_back(&AdaptiveServer::WorkerLoop, this);
    
    accept_thread_ = std::thread(&AdaptiveServer::AcceptLoop, this);
    
    std::cout << "Server started on port " << port_ << std::endl;
    std::cout << "Memory limit: 25% of system RAM" << std::endl;
    return true;
}

void AdaptiveServer::Stop() {
    running_ = false;
    queue_cv_.notify_all();
    
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
    
    for (auto& w : workers_) {
        if (w.joinable()) {
            w.join();
        }
    }
    workers_.clear();
    
    if (server_socket_ >= 0) {
        close(server_socket_);
        server_socket_ = -1;
    }
}

void AdaptiveServer::AcceptLoop() {
    while (running_) {
        int client = accept(server_socket_, nullptr, nullptr);
        if (client >= 0) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            task_queue_.push([this, client]() { HandleClient(client); });
            queue_cv_.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(specs_.optimal_sleep_ms));
    }
}

void AdaptiveServer::WorkerLoop() {
    while (running_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (task_queue_.empty()) {
                queue_cv_.wait_for(lock, std::chrono::milliseconds(100));
                continue;
            }
            task = std::move(task_queue_.front());
            task_queue_.pop();
        }
        if (task) task();
    }
}

void AdaptiveServer::HandleClient(int client_socket) {
    char buffer[8192] = {0};
    recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    std::string response = GenerateResponse(std::string(buffer));
    send(client_socket, response.c_str(), response.length(), 0);
    close(client_socket);
}

std::string AdaptiveServer::GenerateResponse(const std::string& request) {
    std::stringstream ss;
    
    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Type: application/json\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "\r\n";
    ss << "{";
    ss << "\"status\":\"ok\",";
    ss << "\"source\":\"" << god::Watermark::GetSignature() << "\",";
    ss << "\"memory_limit\":25,";
    ss << "\"threads\":" << workers_.size();
    ss << "}";
    
    return ss.str();
}
