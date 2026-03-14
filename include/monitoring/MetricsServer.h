#ifndef METRICS_SERVER_H
#define METRICS_SERVER_H

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <map>

namespace monitoring {

struct SystemMetrics {
    double uptime_seconds;
    double cpu_usage_percent;
    double memory_usage_mb;
    double memory_limit_mb;
    int thread_count;
    int active_sessions;
    int total_requests;
    int error_count;
};

class MetricsServer {
public:
    MetricsServer(int port = 33000);
    ~MetricsServer();
    
    bool Start();
    void Stop();
    
    void RecordRequest();
    void RecordError();
    void SetActiveSessions(int count);
    
    SystemMetrics GetMetrics() const;
    std::string GetPrometheusMetrics();

private:
    int port_;
    int server_socket_;
    std::atomic<bool> running_;
    std::thread http_thread_;
    mutable std::mutex metrics_mutex_;
    
    SystemMetrics current_;
    int64_t start_time_;
    std::atomic<int> total_requests_;
    std::atomic<int> error_count_;
    std::atomic<int> active_sessions_;
    
    void AcceptLoop();
    void HandleRequest(int client_socket);
    void UpdateSystemMetrics();
};

} // namespace monitoring
#endif
