#include "../include/core/AdaptiveServer.h"
#include "../include/god/watermark.h"
#include "../include/monitoring/MetricsServer.h"
#include "../include/ai/RecursiveOrchestrator.h"
#include "../include/system/MemoryLimiter.h"
#include <iostream>
#include <csignal>
#include <memory>
#include <thread>
#include <atomic>

std::unique_ptr<AdaptiveServer> g_server;
std::unique_ptr<monitoring::MetricsServer> g_metrics;
std::unique_ptr<ai::RecursiveOrchestrator> g_orchestrator;
std::atomic<bool> g_running{true};

void SignalHandler(int signum) {
    std::cout << "\nShutting down..." << std::endl;
    g_running = false;
}

void Cleanup() {
    if (g_orchestrator) g_orchestrator->Stop();
    if (g_metrics) g_metrics->Stop();
    if (g_server) g_server->Stop();
    std::cout << "Cleanup complete." << std::endl;
}

int main() {
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    std::cout << "\n========================================\n";
    std::cout << "Fully Recursive Autonomous Appium\n";
    std::cout << "Source: " << god::Watermark::GetSignature() << "\n";
    std::cout << "Version: 4.0 - Complete System\n";
    std::cout << "========================================\n\n";

    // Start AI orchestrator with 333 agents
    g_orchestrator = std::make_unique<ai::RecursiveOrchestrator>();
    g_orchestrator->Start();

    // Start metrics server
    g_metrics = std::make_unique<monitoring::MetricsServer>(33000);
    if (!g_metrics->Start()) {
        std::cerr << "Failed to start metrics server\n";
    }

    // Start main server
    g_server = std::make_unique<AdaptiveServer>(4724);
    if (!g_server->Start()) {
        std::cerr << "Failed to start main server\n";
        Cleanup();
        return 1;
    }

    std::cout << "\n✅ System ready:\n";
    std::cout << "   Appium: http://localhost:4724/\n";
    std::cout << "   Swagger: http://localhost:8080/\n";
    std::cout << "   Metrics: http://localhost:33000/metrics\n";
    std::cout << "   Health: http://localhost:33000/health\n";
    std::cout << "   AI Agents: 333\n";
    std::cout << "   Memory Limit: 25% of system RAM\n";
    std::cout << "\nPress Ctrl+C to stop\n";

    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Cleanup();
    return 0;
}
