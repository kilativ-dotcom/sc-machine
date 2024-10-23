#pragma once

#include <sc-memory/sc_agent.hpp>

namespace connectors
{
class GenerateConnectorsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScAction & action) override;
};

}  // namespace connectors
