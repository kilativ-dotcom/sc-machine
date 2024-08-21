#include "agents_test_utils.hpp"

#include "sc-memory/sc_agent.hpp"

#include "test_sc_agent.hpp"
#include "test_sc_module.hpp"

TEST_F(ScAgentTest, InvalidSubscription)
{
  ScAddr const & subscriptionElementAddr{1233241};
  EXPECT_THROW(m_ctx->SubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr), utils::ExceptionInvalidParams);
  EXPECT_THROW(
      m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr), utils::ExceptionInvalidParams);
}

TEST_F(ScAgentTest, SubscribeAgentTwice)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_NO_THROW(m_ctx->SubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr));
  EXPECT_THROW(m_ctx->SubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr), utils::ExceptionInvalidState);
  EXPECT_NO_THROW(m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr));
}

TEST_F(ScAgentTest, UnsubscribeAgentTwice)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_NO_THROW(m_ctx->SubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr));
  EXPECT_NO_THROW(m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr));
  EXPECT_THROW(
      m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr), utils::ExceptionInvalidState);
}

TEST_F(ScAgentTest, UnsubscribeNotSubscribedAgent)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_THROW(
      m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr), utils::ExceptionInvalidState);
}

TEST_F(ScAgentTest, ATestGenerateConnector)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  m_ctx->SubscribeAgent<ATestGenerateConnector>(subscriptionElementAddr);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, node, subscriptionElementAddr);
  EXPECT_TRUE(e.IsValid());
  EXPECT_TRUE(ATestGenerateConnector::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestGenerateConnector>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestGenerateIncomingArc)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  m_ctx->SubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, node, subscriptionElementAddr);
  EXPECT_TRUE(e.IsValid());
  EXPECT_TRUE(ATestGenerateIncomingArc::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestGenerateIncomingArc>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestGenerateOutgoingArc)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  m_ctx->SubscribeAgent<ATestGenerateOutgoingArc>(subscriptionElementAddr);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, subscriptionElementAddr, node);
  EXPECT_TRUE(e.IsValid());
  EXPECT_TRUE(ATestGenerateOutgoingArc::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestGenerateOutgoingArc>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestGenerateEdge)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);
  m_ctx->SubscribeAgent<ATestGenerateEdge>(subscriptionElementAddr);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeUCommonConst, subscriptionElementAddr, node);
  EXPECT_TRUE(e.IsValid());
  EXPECT_TRUE(ATestGenerateEdge::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestGenerateEdge>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestEraseIncomingArc)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, node, subscriptionElementAddr);
  EXPECT_TRUE(e.IsValid());

  m_ctx->SubscribeAgent<ATestEraseIncomingArc>(subscriptionElementAddr);

  EXPECT_TRUE(m_ctx->EraseElement(e));
  EXPECT_TRUE(ATestEraseIncomingArc::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestEraseIncomingArc>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestEraseOutgoingArc)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, subscriptionElementAddr, node);
  EXPECT_TRUE(e.IsValid());

  m_ctx->SubscribeAgent<ATestEraseOutgoingArc>(subscriptionElementAddr);

  EXPECT_TRUE(m_ctx->EraseElement(e));
  EXPECT_TRUE(ATestEraseOutgoingArc::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestEraseOutgoingArc>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestEraseEdge)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeUCommonConst, subscriptionElementAddr, node);
  EXPECT_TRUE(e.IsValid());

  m_ctx->SubscribeAgent<ATestEraseEdge>(subscriptionElementAddr);

  EXPECT_TRUE(m_ctx->EraseElement(e));
  EXPECT_TRUE(ATestEraseEdge::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestEraseEdge>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestEraseElement)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  m_ctx->SubscribeAgent<ATestEraseElement>(subscriptionElementAddr);

  EXPECT_TRUE(m_ctx->EraseElement(subscriptionElementAddr));
  EXPECT_TRUE(ATestEraseElement::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestEraseElement>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestChangeLinkContent)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateLink(ScType::LinkConst);

  m_ctx->SubscribeAgent<ATestChangeLinkContent>(subscriptionElementAddr);

  uint32_t const value = 100;
  ScStreamPtr const stream = ScStreamMakeRead(value);
  EXPECT_TRUE(m_ctx->SetLinkContent(subscriptionElementAddr, stream));
  EXPECT_TRUE(ATestChangeLinkContent::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestChangeLinkContent>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestGenerateMultipleOutputArc)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  m_ctx->SubscribeAgent<ATestGenerateMultipleOutputArc>(subscriptionElementAddr);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());

  ScAddr const e1 = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, subscriptionElementAddr, node);
  EXPECT_TRUE(e1.IsValid());
  EXPECT_TRUE(ATestGenerateMultipleOutputArc::msWaiter.Wait());

  ScAddr const e2 = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, subscriptionElementAddr, node);
  EXPECT_TRUE(e2.IsValid());
  EXPECT_TRUE(ATestGenerateMultipleOutputArc::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestGenerateMultipleOutputArc>(subscriptionElementAddr);
}

TEST_F(ScAgentTest, ATestCheckResultOnlyFirstArgumentV1)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArgument(1, ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestCheckResult::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestCheckResultOnlyFirstArgumentV2)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  EXPECT_TRUE(m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .SetArgument(1, ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .InitiateAndWait(2000));

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestCheckResultOnlySecondArgumentV1)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArgument(2, ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestCheckResult::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestCheckResultOnlySecondArgumentV2)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  EXPECT_TRUE(m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .SetArgument(2, ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .InitiateAndWait(2000));

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestCheckResultTwoArgumentsV1)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArguments(
          ATestGenerateOutgoingArc::generate_outgoing_arc_action,
          ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestCheckResult::msWaiter.Wait());

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestCheckResultTwoArgumentsV2)
{
  m_ctx->SubscribeAgent<ATestCheckResult>();

  EXPECT_TRUE(m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .SetArguments(
                      ATestGenerateOutgoingArc::generate_outgoing_arc_action,
                      ATestGenerateOutgoingArc::generate_outgoing_arc_action)
                  .InitiateAndWait(2000));

  m_ctx->UnsubscribeAgent<ATestCheckResult>();
}

TEST_F(ScAgentTest, ATestGetInitiationConditionTemplate)
{
  m_ctx->SubscribeAgent<ATestGetInitiationConditionTemplate>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_FALSE(ATestGetInitiationConditionTemplate::msWaiter.Wait(0.2));

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArguments(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestGetInitiationConditionTemplate::msWaiter.Wait(1));

  m_ctx->UnsubscribeAgent<ATestGetInitiationConditionTemplate>();
}

TEST_F(ScAgentTest, ATestCheckInitiationCondition)
{
  m_ctx->SubscribeAgent<ATestCheckInitiationCondition>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_FALSE(ATestCheckInitiationCondition::msWaiter.Wait(0.2));

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArguments(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestCheckInitiationCondition::msWaiter.Wait(1));

  m_ctx->UnsubscribeAgent<ATestCheckInitiationCondition>();
}

TEST_F(ScAgentTest, ATestGetResultConditionTemplate)
{
  m_ctx->SubscribeAgent<ATestGetResultConditionTemplate>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_TRUE(ATestGetResultConditionTemplate::msWaiter.Wait(1));

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArguments(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestGetResultConditionTemplate::msWaiter.Wait(1));

  m_ctx->UnsubscribeAgent<ATestGetResultConditionTemplate>();
}

TEST_F(ScAgentTest, ATestCheckResultCondition)
{
  m_ctx->SubscribeAgent<ATestCheckResultCondition>();

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_TRUE(ATestCheckResultCondition::msWaiter.Wait(1));

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .SetArguments(ATestGenerateOutgoingArc::generate_outgoing_arc_action)
      .Initiate();

  EXPECT_TRUE(ATestCheckResultCondition::msWaiter.Wait(1));

  m_ctx->UnsubscribeAgent<ATestCheckResultCondition>();
}

TEST_F(ScAgentTest, ActionDeactivated)
{
  m_ctx->SubscribeAgent<ATestActionDeactivated>();

  ScAddr const & arcAddr = m_ctx->CreateEdge(
      ScType::EdgeAccessConstPosPerm,
      ScKeynodes::action_deactivated,
      ATestGenerateOutgoingArc::generate_outgoing_arc_action);

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_FALSE(ATestActionDeactivated::msWaiter.Wait(0.2));

  m_ctx->EraseElement(arcAddr);

  m_ctx->CreateAction(ATestGenerateOutgoingArc::generate_outgoing_arc_action).SetArguments().Initiate();

  EXPECT_TRUE(ATestActionDeactivated::msWaiter.Wait(1));

  m_ctx->UnsubscribeAgent<ATestActionDeactivated>();
}

TEST_F(ScAgentTest, RegisterAgentWithinModule)
{
  ScAddr const & subscriptionElementAddr = m_ctx->CreateNode(ScType::NodeConst);

  TestModule module;
  module.Agent<ATestGenerateOutgoingArc>(subscriptionElementAddr);
  module.Register(&*m_ctx);

  ScAddr const node = m_ctx->CreateNode(ScType::NodeConst);
  EXPECT_TRUE(node.IsValid());
  ScAddr const e = m_ctx->CreateEdge(ScType::EdgeAccessConstPosPerm, subscriptionElementAddr, node);
  EXPECT_TRUE(e.IsValid());
  EXPECT_TRUE(ATestGenerateOutgoingArc::msWaiter.Wait());

  module.Unregister(&*m_ctx);
}

TEST_F(ScAgentTest, AgentHasNoSpecificationInKb)
{
  ATestCheckResult agent;
  EXPECT_THROW(agent.GetAbstractAgent(), utils::ExceptionInvalidState);
  EXPECT_NO_THROW(agent.GetActionClass());
  EXPECT_EQ(agent.GetEventClass(), ScKeynodes::sc_event_generate_outgoing_arc);
  EXPECT_THROW(agent.GetEventSubscriptionElement(), utils::ExceptionInvalidState);
  EXPECT_THROW(agent.GetInitiationCondition(), utils::ExceptionInvalidState);
  EXPECT_THROW(agent.GetResultCondition(), utils::ExceptionInvalidState);
  EXPECT_NO_THROW(agent.GetInitiationConditionTemplate());
  EXPECT_NO_THROW(agent.GetResultConditionTemplate());
  EXPECT_EQ(agent.GetName(), "ATestCheckResult");
}
