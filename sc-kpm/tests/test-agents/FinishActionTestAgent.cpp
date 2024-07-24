#include "FinishActionTestAgent.hpp"

using namespace scUtilsTestAgents;

SC_AGENT_IMPLEMENTATION(FinishActionTestAgent)
{
  ScTemplate actionDescriptionTemplate;
  actionDescriptionTemplate.Quintuple(
      otherAddr, ScType::EdgeDCommonVar, ScType::NodeVarStruct, ScType::EdgeAccessVarPosPerm, scAgentsCommon::CoreKeynodes::nrel_answer);
  actionDescriptionTemplate.Triple(
      scAgentsCommon::CoreKeynodes::action_finished, ScType::EdgeAccessVarPosPerm, otherAddr);
  ScTemplateGenResult actionDescriptionGenResult;
  m_memoryCtx.HelperGenTemplate(actionDescriptionTemplate, actionDescriptionGenResult);

  return SC_RESULT_OK;
}
