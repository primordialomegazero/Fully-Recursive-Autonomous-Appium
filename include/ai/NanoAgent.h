#ifndef NANO_AGENT_H
#define NANO_AGENT_H

namespace ai {
class NanoAgent {
public:
    NanoAgent(int id);
    ~NanoAgent();
    int GetId() const { return id_; }
private:
    int id_;
};
} // namespace ai
#endif
