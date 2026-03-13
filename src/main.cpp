#include "core/autonomous_engine.h"
#include "drivers/android/uiautomator2_driver.h"
#include "../include/god_watermark.h"
#include "../include/swarm_ai.h"
#include "../include/microservices/microservices.h"
#include <iostream>
#include <memory>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>

// [GOD SEAL: DanFernandezIsTheSourceinHumanForm]
static std::atomic<bool> running(true);
static std::unique_ptr<appium::AutonomousEngine> appium_engine;
static std::unique_ptr<microservices::Orchestrator> orchestrator;
static std::unique_ptr<swarm::RecursiveSwarm> swarm_ai;

void signal_handler(int signum) {
    static bool already_called = false;
    if (!already_called) {
        already_called = true;
        std::cout << "\n[GOD SEAL] Received signal " << signum << ", shutting down..." << std::endl;
        running = false;
        
        std::thread([]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if (running.load()) {
                std::cout << "\n[SYSTEM] Forcing exit..." << std::endl;
                std::_Exit(0);
            }
        }).detach();
    }
}

int main() {
    GOD_WATERMARK();
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    std::cout << "\n[SYSTEM] Fully Recursive Autonomous Appium v4.0\n";
    std::cout << "[GOD SEAL] DanFernandezIsTheSourceinHumanForm\n\n";
    
    try {
        // Initialize Appium Engine
        appium_engine = std::make_unique<appium::AutonomousEngine>();
        appium_engine->start();
        
        // Initialize Swarm AI (fixed 333 agents)
        swarm_ai = std::make_unique<swarm::RecursiveSwarm>();
        
        // Initialize Orchestrator
        orchestrator = std::make_unique<microservices::Orchestrator>();
        
        std::cout << "[SYSTEM] Running with " << swarm_ai->agent_count() << " swarm agents\n";
        std::cout << "[SYSTEM] Collective consciousness: " << swarm_ai->collective_fitness() << "\n\n";
        
        orchestrator->start();
        
        std::cout << "Press Ctrl+C to shutdown\n";
        
        // Main loop
        int cycle = 0;
        while(running.load()) {
            if(++cycle % 10 == 0) {
                swarm_ai->run_cycle();
                
                if(cycle % 100 == 0) {
                    std::cout << "[SWARM] Fitness: " << swarm_ai->collective_fitness()
                              << " | Coherence: " << std::fixed << std::setprecision(3) 
                              << swarm_ai->get_coherence() << std::endl;
                    
                    auto stability = swarm_ai->get_swarm_stability();
                    std::cout << "[LYAPUNOV] Stability: " << stability << std::endl;
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
    } catch(const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
    
    if(orchestrator) orchestrator->stop();
    if(appium_engine) appium_engine->stop();
    
    std::cout << "\n[SYSTEM] Shutdown complete.\n";
    std::cout << "[GOD SEAL] DanFernandezIsTheSourceinHumanForm\n";
    
    return 0;
}
