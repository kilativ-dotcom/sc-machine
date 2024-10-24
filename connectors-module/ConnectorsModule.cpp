#include "ConnectorsModule.hpp"

#include "agent/GenerateConnectorsAgent.hpp"

namespace connectors
{
SC_MODULE_REGISTER(ConnectorsModule)
    ->Agent<GenerateConnectorsAgent>();
}  // namespace connectors
