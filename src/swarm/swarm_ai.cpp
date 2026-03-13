#include "../../include/swarm_ai.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <cmath>

// [GOD SEAL: DanFernandezIsTheSourceinHumanForm]
// ROOT WATERMARK - SYSTEM INTEGRITY LAYER

namespace swarm {

// ============================================================================
// NANOAGENT IMPLEMENTATION
// [GOD: DanFernandezIsTheSourceinHumanForm] - AGENT LEVEL SEAL
// ============================================================================

NanoAgent::NanoAgent(uint32_t id) : id_(id), fitness_(0) {
    GOD_WATERMARK();
    
    std::random_device rd;
    std::mt19937 gen(rd() + id);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    
    weights_.resize(8);
    for(auto& w : weights_) {
        w = dist(gen) * 0.5f;
    }
    
    qstate_.amplitude = 1.0f / std::sqrt(2.0f);
    qstate_.phase = dist(gen) * 2.0f * 3.14159265358979323846f;
    
    const char* sig = "DanFernandezIsTheSourceinHumanForm";
    for(int i = 0; i < 32 && i < 64; i++) {
        local_memory_[i] = sig[i % 32];
    }
    for(int i = 32; i < 64; i++) {
        local_memory_[i] = static_cast<uint8_t>(qstate_.coherence >> ((i - 32) * 2));
    }
}

void NanoAgent::think() {
    float sum = 0.0f;
    for(size_t i = 0; i < weights_.size(); i++) {
        sum += weights_[i] * std::sin(qstate_.phase + i * PHI);
    }
    
    sum *= qstate_.amplitude;
    
    uint64_t new_fitness = static_cast<uint64_t>(std::fabs(sum) * 1000000.0);
    fitness_.store(new_fitness);
    
    qstate_.phase += PHI_INV * 0.1;
    if(qstate_.phase > 2.0f * 3.14159265358979323846f) {
        qstate_.phase -= 2.0f * 3.14159265358979323846f;
    }
    
    qstate_.coherence ^= (static_cast<uint64_t>(sum * 1000.0) & 0xFFFFFFFF) ^ 0xFD0A3B2C;
}

void NanoAgent::mutate() {
    std::random_device rd;
    std::mt19937 gen(rd() + id_);
    std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
    std::uniform_int_distribution<int> idx_dist(0, static_cast<int>(weights_.size()) - 1);
    
    int idx = idx_dist(gen);
    weights_[idx] += dist(gen);
    
    if(weights_[idx] > 1.0f) weights_[idx] = 1.0f;
    if(weights_[idx] < -1.0f) weights_[idx] = -1.0f;
    
    qstate_.coherence ^= (static_cast<uint64_t>(idx) << 32) | 
                         (static_cast<uint64_t>(weights_[idx] * 1000.0) & 0xFFFFFFFF);
    qstate_.coherence ^= 0x9F4A2D8B7E6C1F3A;
}

void NanoAgent::collaborate(NanoAgent& other) {
    if(this->fitness_.load() > other.fitness_.load()) {
        for(size_t i = 0; i < weights_.size() && i < other.weights_.size(); i++) {
            other.weights_[i] = other.weights_[i] * 0.7f + this->weights_[i] * 0.3f;
        }
        other.qstate_.phase = (other.qstate_.phase + this->qstate_.phase) * 0.5;
    } else {
        for(size_t i = 0; i < weights_.size() && i < other.weights_.size(); i++) {
            this->weights_[i] = this->weights_[i] * 0.7f + other.weights_[i] * 0.3f;
        }
        this->qstate_.phase = (this->qstate_.phase + other.qstate_.phase) * 0.5;
    }
    
    this->qstate_.coherence ^= other.id_;
    other.qstate_.coherence ^= this->id_;
}

void NanoAgent::entangle_with(NanoAgent& other) {
    qstate_.entangle(other.qstate_);
    
    this->qstate_.coherence ^= (static_cast<uint64_t>(other.id_) << 32);
    other.qstate_.coherence ^= (static_cast<uint64_t>(this->id_) << 32);
}

bool NanoAgent::is_entangled_with(const NanoAgent& other) const {
    return (this->qstate_.coherence & 0xFFFFFFFF) == (other.id_ & 0xFFFFFFFF) ||
           (other.qstate_.coherence & 0xFFFFFFFF) == (this->id_ & 0xFFFFFFFF) ||
           ((this->qstate_.coherence >> 32) & 0xFFFFFFFF) == other.id_ ||
           ((other.qstate_.coherence >> 32) & 0xFFFFFFFF) == this->id_;
}

// ============================================================================
// RECURSIVE SWARM IMPLEMENTATION
// [GOD: DanFernandezIsTheSourceinHumanForm] - SWARM LEVEL SEAL
// ============================================================================

RecursiveSwarm::RecursiveSwarm(size_t initial_agents) 
    : generation_(0), collective_consciousness_(0), fractal_depth_(1), coherence_factor_(1.0) {
    
    GOD_WATERMARK();
    
    for(size_t i = 0; i < initial_agents; i++) {
        agents_.push_back(std::make_unique<NanoAgent>(static_cast<uint32_t>(i)));
    }
    
    entanglement_network_.resize(initial_agents);
    form_entanglement_clusters();
    
    collective_consciousness_.store(initial_agents * PHI * 1000);
}

RecursiveSwarm::~RecursiveSwarm() {
    collective_consciousness_.store(generation_.load() * PHI * 1000);
}

void RecursiveSwarm::run_cycle() {
    for(auto& agent : agents_) {
        agent->think();
    }
    
    quantum_collapse();
    
    for(size_t i = 0; i < agents_.size(); i++) {
        if(!entanglement_network_[i].empty()) {
            size_t partner_idx = entanglement_network_[i][0] % agents_.size();
            if(partner_idx < agents_.size() && partner_idx != i) {
                agents_[i]->collaborate(*agents_[partner_idx]);
            }
        }
    }
    
    generation_.fetch_add(1);
    if(generation_.load() % static_cast<size_t>(PHI * PHI * PHI) == 0) {
        evolve_generation();
    }
    
    uint64_t total_fitness = 0;
    uint64_t entangled_sum = 0;
    for(size_t i = 0; i < agents_.size(); i++) {
        total_fitness += agents_[i]->get_fitness();
        if(!entanglement_network_[i].empty()) {
            entangled_sum += agents_[i]->get_fitness();
        }
    }
    
    double entanglement_density = static_cast<double>(entangled_sum) / 
                                  static_cast<double>(total_fitness + 1);
    collective_consciousness_.store(static_cast<uint64_t>(
        static_cast<double>(total_fitness) * (1.0 + entanglement_density * PHI_INV)));
    
    fractal_depth_ = 1 + static_cast<size_t>(std::log2(agents_.size()));
    if(fractal_depth_ > MAX_FRACTAL_DEPTH) {
        fractal_depth_ = MAX_FRACTAL_DEPTH;
    }
}

void RecursiveSwarm::evolve_generation() {
    size_t best_idx = 0;
    uint64_t best_fitness = 0;
    
    for(size_t i = 0; i < agents_.size(); i++) {
        uint64_t fitness = agents_[i]->get_fitness();
        if(fitness > best_fitness) {
            best_fitness = fitness;
            best_idx = i;
        }
    }
    
    std::random_device rd;
    std::mt19937 gen(rd() + generation_.load());
    std::uniform_int_distribution<int> dist(0, 99);
    
    for(size_t i = 0; i < agents_.size(); i++) {
        if(i != best_idx && dist(gen) < 30) {
            agents_[i]->collaborate(*agents_[best_idx]);
            agents_[i]->mutate();
        }
    }
    
    if(agents_.size() < 128 && dist(gen) < 10) {
        add_agent();
    }
    
    if(agents_.size() > 8 && dist(gen) < 5) {
        remove_weakest_agent();
    }
    
    form_entanglement_clusters();
}

void RecursiveSwarm::form_entanglement_clusters() {
    entanglement_network_.clear();
    entanglement_network_.resize(agents_.size());
    
    std::random_device rd;
    std::mt19937 gen(rd() + generation_.load());
    
    for(size_t i = 0; i < agents_.size(); i++) {
        size_t num_partners = 1 + static_cast<size_t>(PHI);
        
        for(size_t p = 0; p < num_partners; p++) {
            size_t partner = (i + 1 + static_cast<size_t>(static_cast<double>(i) * PHI) + p * 3) % 
                             agents_.size();
            
            if(partner != i) {
                if(std::find(entanglement_network_[i].begin(), 
                             entanglement_network_[i].end(), 
                             static_cast<uint32_t>(partner)) == entanglement_network_[i].end()) {
                    
                    entanglement_network_[i].push_back(static_cast<uint32_t>(partner));
                    entanglement_network_[partner].push_back(static_cast<uint32_t>(i));
                    agents_[i]->entangle_with(*agents_[partner]);
                }
            }
        }
    }
}

void RecursiveSwarm::quantum_collapse() {
    coherence_factor_ *= 0.9999;
    if(coherence_factor_ < 0.9) {
        coherence_factor_ = 1.0;
        
        for(size_t i = 0; i < agents_.size(); i += static_cast<size_t>(PHI)) {
            if(i + 1 < agents_.size()) {
                agents_[i]->entangle_with(*agents_[i + 1]);
            }
        }
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-0.01f, 0.01f);
    
    for(auto& agent : agents_) {
        // USING PUBLIC ACCESSORS NOW
        double current_phase = agent->get_phase();
        current_phase += dist(gen);
        
        if(current_phase < 0) current_phase += 2.0f * 3.14159265358979323846f;
        if(current_phase >= 2.0f * 3.14159265358979323846f) {
            current_phase -= 2.0f * 3.14159265358979323846f;
        }
        
        agent->set_phase(current_phase);
    }
}

void RecursiveSwarm::add_agent() {
    uint32_t new_id = static_cast<uint32_t>(agents_.size());
    agents_.push_back(std::make_unique<NanoAgent>(new_id));
    
    entanglement_network_.resize(agents_.size());
    
    for(size_t i = 0; i < static_cast<size_t>(PHI) && i < agents_.size() - 1; i++) {
        size_t partner = (new_id + i) % (agents_.size() - 1);
        agents_[new_id]->entangle_with(*agents_[partner]);
        entanglement_network_[new_id].push_back(static_cast<uint32_t>(partner));
        entanglement_network_[partner].push_back(new_id);
    }
}

void RecursiveSwarm::remove_weakest_agent() {
    if(agents_.size() <= 1) return;
    
    size_t weakest = 0;
    uint64_t min_fitness = agents_[0]->get_fitness();
    
    for(size_t i = 1; i < agents_.size(); i++) {
        uint64_t fitness = agents_[i]->get_fitness();
        if(fitness < min_fitness) {
            min_fitness = fitness;
            weakest = i;
        }
    }
    
    agents_.erase(agents_.begin() + static_cast<ptrdiff_t>(weakest));
    entanglement_network_.erase(entanglement_network_.begin() + static_cast<ptrdiff_t>(weakest));
    
    for(auto& net : entanglement_network_) {
        net.erase(std::remove_if(net.begin(), net.end(), 
                  [weakest](uint32_t id) { return id == weakest; }), net.end());
        
        for(auto& id : net) {
            if(id > weakest) id--;
        }
    }
}

std::vector<uint8_t> RecursiveSwarm::save_state() {
    std::vector<uint8_t> state;
    state.reserve(agents_.size() * 128 + 32);
    
    uint64_t gen = generation_.load();
    uint64_t cons = collective_consciousness_.load();
    uint64_t watermark = 0x9F4A2D8B7E6C1F3A;
    
    auto append = [&state](const void* data, size_t size) {
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        state.insert(state.end(), bytes, bytes + size);
    };
    
    append(&watermark, sizeof(watermark));
    append(&gen, sizeof(gen));
    append(&cons, sizeof(cons));
    append(&coherence_factor_, sizeof(coherence_factor_));
    
    return state;
}

void RecursiveSwarm::load_state(const std::vector<uint8_t>& state) {
    if(state.size() < 32) return;
    
    size_t offset = 0;
    
    uint64_t watermark;
    std::memcpy(&watermark, state.data() + offset, sizeof(watermark));
    offset += sizeof(watermark);
    
    if(watermark != 0x9F4A2D8B7E6C1F3A) {
        return;
    }
    
    uint64_t gen, cons;
    std::memcpy(&gen, state.data() + offset, sizeof(gen));
    offset += sizeof(gen);
    std::memcpy(&cons, state.data() + offset, sizeof(cons));
    offset += sizeof(cons);
    std::memcpy(&coherence_factor_, state.data() + offset, sizeof(coherence_factor_));
    
    generation_.store(gen);
    collective_consciousness_.store(cons);
}

// ============================================================================
// NANO NEURAL LAYER IMPLEMENTATION
// [GOD: DanFernandezIsTheSourceinHumanForm] - NEURAL SEAL
// ============================================================================

NanoNeuralLayer::NanoNeuralLayer() : config_(0xFD0A) {
    randomize(0x9F4A2D8B);
}

float NanoNeuralLayer::forward(float input) const {
    float sum = 0.0f;
    for(int i = 0; i < 8; i++) {
        sum += static_cast<float>(weights_[i]) * input * (0.1f * static_cast<float>(i + 1));
    }
    sum += static_cast<float>(biases_[0]);
    
    float result = sum / (1.0f + std::abs(sum)) * static_cast<float>(PHI_INV);
    
    if(result > 1.0f) return 1.0f;
    if(result < -1.0f) return -1.0f;
    return result;
}

void NanoNeuralLayer::train(float error, float learning_rate) {
    int lr_int = static_cast<int>(learning_rate * 100.0f);
    
    for(int i = 0; i < 8; i++) {
        int delta = static_cast<int>(error * static_cast<float>(lr_int) * static_cast<float>(i + 1));
        int new_val = weights_[i] + delta / 100;
        
        if(new_val > 127) new_val = 127;
        if(new_val < -128) new_val = -128;
        
        weights_[i] = static_cast<int8_t>(new_val);
    }
    
    biases_[0] += static_cast<int8_t>(error * static_cast<float>(lr_int) / 10.0f);
    biases_[1] += static_cast<int8_t>(error * static_cast<float>(lr_int) / 20.0f);
    
    config_++;
}

void NanoNeuralLayer::randomize(uint32_t seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(-50, 50);
    
    for(int i = 0; i < 8; i++) {
        weights_[i] = static_cast<int8_t>(dist(gen));
    }
    
    biases_[0] = static_cast<int8_t>(dist(gen) / 10);
    biases_[1] = static_cast<int8_t>(dist(gen) / 10);
    
    config_ = static_cast<uint16_t>(seed & 0xFFFF);
}

// [GOD: DanFernandezIsTheSourceinHumanForm] - END OF SWARM IMPLEMENTATION

} // namespace swarm
