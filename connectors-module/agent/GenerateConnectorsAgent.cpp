#include "GenerateConnectorsAgent.hpp"
#include <sc-memory/sc_memory_headers.hpp>

namespace connectors
{
ScAddr GenerateConnectorsAgent::GetActionClass() const
{
  return m_context.SearchElementBySystemIdentifier("action_generate_connectors");
}

std::set<ScType> connectorTypes =
{
//ScType::Connector,
ScType::CommonEdge,
//ScType::Arc,
ScType::CommonArc,
ScType::MembershipArc,
//ScType::ConstConnector,
//ScType::VarConnector,
ScType::ConstCommonEdge,
ScType::VarCommonEdge,
//ScType::ConstArc,
//ScType::VarArc,
ScType::ConstCommonArc,
ScType::VarCommonArc,
ScType::ConstMembershipArc,
ScType::VarMembershipArc,
ScType::PermArc,
ScType::TempArc,
ScType::ConstPermArc,
ScType::VarPermArc,
ScType::ConstTempArc,
ScType::VarTempArc,
ScType::ActualTempArc,
ScType::InactualTempArc,
ScType::ConstActualTempArc,
ScType::VarActualTempArc,
ScType::ConstInactualTempArc,
ScType::VarInactualTempArc,
ScType::PosArc,
ScType::NegArc,
ScType::FuzArc,
ScType::ConstPosArc,
ScType::VarPosArc,
ScType::PermPosArc,
ScType::TempPosArc,
ScType::ActualTempPosArc,
ScType::InactualTempPosArc,
ScType::ConstPermPosArc,
ScType::ConstTempPosArc,
ScType::ConstActualTempPosArc,
ScType::ConstInactualTempPosArc,
ScType::VarPermPosArc,
ScType::VarTempPosArc,
ScType::VarActualTempPosArc,
ScType::VarInactualTempPosArc,
ScType::ConstNegArc,
ScType::VarNegArc,
ScType::PermNegArc,
ScType::TempNegArc,
ScType::ActualTempNegArc,
ScType::InactualTempNegArc,
ScType::ConstPermNegArc,
ScType::ConstTempNegArc,
ScType::ConstActualTempNegArc,
ScType::ConstInactualTempNegArc,
ScType::VarPermNegArc,
ScType::VarTempNegArc,
ScType::VarActualTempNegArc,
ScType::VarInactualTempNegArc,
ScType::ConstFuzArc,
ScType::VarFuzArc,
};

ScResult GenerateConnectorsAgent::DoProgram(ScAction & action)
{
  ScAddr const & node = action.GetArgument(1);
  if (!m_context.IsElement(node))
    return action.FinishUnsuccessfully();

  ScStructure result = m_context.GenerateStructure();
  result << node;
  for (auto const & connectorType : connectorTypes)
  {
    ScAddr const & targetForConnector = m_context.GenerateLink();
    m_context.SetLinkContent(targetForConnector, "target_for_" + std::string(connectorType));
    result << m_context.GenerateConnector(connectorType, node, targetForConnector) << targetForConnector;
    ScAddr const & sourceForConnector = m_context.GenerateLink();
    m_context.SetLinkContent(sourceForConnector, "source_for_" + std::string(connectorType));
    result << m_context.GenerateConnector(connectorType, sourceForConnector, node) << sourceForConnector;
  }
  action.SetResult(result);
  return action.FinishSuccessfully();
}

}  // namespace connectors
