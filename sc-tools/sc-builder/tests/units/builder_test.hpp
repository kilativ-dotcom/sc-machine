#pragma once

#include <gtest/gtest.h>

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_agent_context.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "test_defines.hpp"

#include <memory>

class ScBuilderTest : public testing::Test
{
protected:
  virtual void SetUp()
  {
    ScBuilderTest::Initialize();
    m_ctx = std::make_unique<ScAgentContext>();
  }

  virtual void TearDown()
  {
    if (m_ctx)
      m_ctx->Destroy();

    ScBuilderTest::Shutdown();
  }

  static void Initialize()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = SC_FALSE;
    params.dump_memory_statistics = SC_FALSE;

    params.clear = SC_FALSE;
    params.repo_path = SC_BUILDER_KB_BIN;

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  void InitializeWithUserMode()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = SC_FALSE;
    params.dump_memory_statistics = SC_FALSE;

    params.clear = SC_FALSE;
    params.repo_path = SC_BUILDER_KB_BIN;

    params.user_mode = SC_TRUE;

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(SC_FALSE);
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
};

class TestScMemoryContext : public ScAgentContext
{
public:
  TestScMemoryContext(ScAddr const & userAddr = ScAddr::Empty)
    : ScAgentContext(userAddr)
  {
  }
};

class ScBuilderLoadUserPermissionsTest : public ScBuilderTest
{
protected:
  virtual void SetUp()
  {
    ScBuilderTest::InitializeWithUserMode();
    m_ctx = std::make_unique<TestScMemoryContext>(ScKeynodes::myself);
  }
};
