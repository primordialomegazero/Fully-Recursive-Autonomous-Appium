#ifndef AGENT_SWARM_H
#define AGENT_SWARM_H

namespace ai {
class AgentSwarm {
public:
    AgentSwarm(int num_agents = 333);
    ~AgentSwarm();
    void Start();
    void Stop();
};
} // namespace ai
#endif
