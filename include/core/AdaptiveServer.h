#ifndef ADAPTIVE_SERVER_H
#define ADAPTIVE_SERVER_H

#include "../system/SystemDetector.h"
#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>

class AdaptiveServer {
public:
    explicit AdaptiveServer(int port = 4723);
    ~AdaptiveServer();
    bool Start();
    void Stop();
    bool IsRunning() const { return running_; }

private:
    int port_;
    int server_socket_;
    std::atomic<bool> running_;
    SystemSpecs specs_;
    
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::thread accept_thread_;
    
    void AcceptLoop();
    void WorkerLoop();
    void HandleClient(int client_socket);
    std::string GenerateResponse(const std::string& request);
};

#endif
