#include "../../include/security/QuantumSecurity.h"

namespace quantum {
QuantumSecurity::QuantumSecurity() {}
QuantumSecurity::~QuantumSecurity() {}
bool QuantumSecurity::ValidateWatermark(const std::string&) { return true; }
} // namespace quantum
