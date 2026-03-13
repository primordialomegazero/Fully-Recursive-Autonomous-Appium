#include "autonomous_engine.h"
#include <iostream>

namespace appium {

AutonomousEngine::AutonomousEngine(size_t num_workers) 
    : running_(false), recursion_depth_(0) {
    
    task_queue_ = std::make_unique<RecursiveQueue<std::function<void()>>>();
    (void)num_workers; // Suppress unused parameter warning
}

AutonomousEngine::~AutonomousEngine() {
    stop();
    for(auto& worker : worker_pool_) {
        if(worker.joinable()) {
            worker.join();
        }
    }
}

void AutonomousEngine::worker_loop() {
    while(running_.load()) {
        try {
            auto task = task_queue_->pop();
            if(task) {
                recursion_depth_.fetch_add(1);
                task();
                recursion_depth_.fetch_sub(1);
            }
        } catch(const std::exception& e) {
            std::cerr << "Worker error: " << e.what() << std::endl;
        }
    }
}

void AutonomousEngine::submit_task(std::function<void()> task) {
    if(task && running_.load()) {
        task_queue_->push(task);
    }
}

void AutonomousEngine::start() {
    running_.store(true);
    
    size_t num_workers = std::thread::hardware_concurrency();
    if(num_workers == 0) num_workers = 2;
    
    for(size_t i = 0; i < num_workers; i++) {
        worker_pool_.emplace_back(&AutonomousEngine::worker_loop, this);
    }
}

void AutonomousEngine::stop() {
    running_.store(false);
    
    // Push empty tasks to wake up workers
    for(size_t i = 0; i < worker_pool_.size(); i++) {
        task_queue_->push(nullptr);
    }
}

} // namespace appium
