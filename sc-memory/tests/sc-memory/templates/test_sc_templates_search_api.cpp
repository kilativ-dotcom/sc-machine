#include <gtest/gtest.h>

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_structure.hpp"

#include "template_test_utils.hpp"

using ScTemplateSearchApiTest = ScTemplateTest;

TEST_F(ScTemplateSearchApiTest, SearchWithResultNotSafeGet)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const arcAddr = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(arcAddr.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  ScTemplateSearchResult result;
  EXPECT_TRUE(m_ctx->HelperSearchTemplate(templ, result));
  EXPECT_EQ(result.Size(), 1u);

  ScTemplateSearchResultItem const & item = result[0];

  EXPECT_EQ(item[0], addr1);
  EXPECT_EQ(item["_addr1"], addr1);
  EXPECT_EQ(item[1], arcAddr);
  EXPECT_EQ(item["_edge"], arcAddr);
  EXPECT_EQ(item[2], addr2);
  EXPECT_EQ(item["_addr2"], addr2);

  EXPECT_TRUE(item.Has("_addr1"));
  EXPECT_TRUE(item.Has("_edge"));
  EXPECT_TRUE(item.Has("_addr2"));
  EXPECT_FALSE(item.Has("_other_edge"));

  EXPECT_THROW(result[-1], utils::ExceptionInvalidParams);
  EXPECT_THROW(result[1], utils::ExceptionInvalidParams);
  EXPECT_THROW(item["_other_edge"], utils::ExceptionInvalidParams);

  EXPECT_THROW(item[-1], utils::ExceptionInvalidParams);
  EXPECT_THROW(item[3], utils::ExceptionInvalidParams);
}

TEST_F(ScTemplateSearchApiTest, SearchWithResultSafeGet)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const arcAddr = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(arcAddr.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  ScTemplateSearchResult result;
  EXPECT_TRUE(m_ctx->HelperSearchTemplate(templ, result));
  EXPECT_EQ(result.Size(), 1u);

  ScTemplateSearchResultItem item;
  EXPECT_TRUE(result.Get(0, item));
  ScAddr foundAddr;

  EXPECT_TRUE(item.Get(0, foundAddr));
  EXPECT_EQ(foundAddr, addr1);
  EXPECT_TRUE(item.Get("_addr1", foundAddr));
  EXPECT_EQ(foundAddr, addr1);

  EXPECT_TRUE(item.Get(1, foundAddr));
  EXPECT_EQ(foundAddr, arcAddr);
  EXPECT_TRUE(item.Get("_edge", foundAddr));
  EXPECT_EQ(foundAddr, arcAddr);

  EXPECT_TRUE(item.Get(2, foundAddr));
  EXPECT_EQ(foundAddr, addr2);
  EXPECT_TRUE(item.Get("_addr2", foundAddr));
  EXPECT_EQ(foundAddr, addr2);

  EXPECT_FALSE(item.Get(3, foundAddr));
  EXPECT_FALSE(foundAddr.IsValid());
  EXPECT_FALSE(item.Get("_other_edge", foundAddr));
  EXPECT_FALSE(foundAddr.IsValid());

  EXPECT_FALSE(result.Get(1, item));
}

TEST_F(ScTemplateSearchApiTest, SearchWithCallback)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const arcAddr = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(arcAddr.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const & item)
      {
        ScAddr foundAddr;

        EXPECT_TRUE(item.Get(0, foundAddr));
        EXPECT_EQ(foundAddr, addr1);
        EXPECT_TRUE(item.Get("_addr1", foundAddr));
        EXPECT_EQ(foundAddr, addr1);

        EXPECT_TRUE(item.Get(1, foundAddr));
        EXPECT_EQ(foundAddr, arcAddr);
        EXPECT_TRUE(item.Get("_edge", foundAddr));
        EXPECT_EQ(foundAddr, arcAddr);

        EXPECT_TRUE(item.Get(2, foundAddr));
        EXPECT_EQ(foundAddr, addr2);
        EXPECT_TRUE(item.Get("_addr2", foundAddr));
        EXPECT_EQ(foundAddr, addr2);

        ++count;
      });

  EXPECT_EQ(count, 1u);
}

TEST_F(ScTemplateSearchApiTest, SearchWithCallbackForLoop)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const arcAddr = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(arcAddr.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const & item)
      {
        for (ScAddr const & addr : item)
        {
          EXPECT_TRUE(addr.IsValid());
        }

        ++count;
      });

  EXPECT_EQ(count, 1u);
}

TEST_F(ScTemplateSearchApiTest, SearchWithCallbackAndCheck)
{
  ScAddr const & addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const & addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const & arcAddr = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(arcAddr.IsValid());

  ScAddr const & structureAddr = m_ctx->GenerateNode(ScType::NodeConstStruct);
  m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, structureAddr, addr1);
  m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, structureAddr, addr2);
  m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, structureAddr, arcAddr);

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScTemplateSearchResultItem const & item) -> bool
      {
        return !m_ctx->HelperCheckEdge(addr1, item["_addr2"], ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 0u);

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScTemplateSearchResultItem const & item) -> bool
      {
        return m_ctx->HelperCheckEdge(addr1, item["_addr2"], ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 1u);
  count = 0;

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScAddr const & addr) -> bool
      {
        return m_ctx->HelperCheckEdge(addr1, addr, ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 0u);

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScAddr const & addr) -> bool
      {
        return m_ctx->HelperCheckEdge(structureAddr, addr, ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 1u);
  count = 0;

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScTemplateSearchResultItem const & item) -> bool
      {
        return m_ctx->HelperCheckEdge(addr1, item["_addr2"], ScType::EdgeAccessConstPosPerm);
      },
      [&](ScAddr const & addr) -> bool
      {
        return m_ctx->HelperCheckEdge(structureAddr, addr, ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 1u);
  count = 0;

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScTemplateSearchResultItem const & item) -> bool
      {
        return !m_ctx->HelperCheckEdge(addr1, item["_addr2"], ScType::EdgeAccessConstPosPerm);
      },
      [&](ScAddr const & addr) -> bool
      {
        return m_ctx->HelperCheckEdge(structureAddr, addr, ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 0u);

  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      },
      [&](ScTemplateSearchResultItem const & item) -> bool
      {
        return m_ctx->HelperCheckEdge(addr1, item["_addr2"], ScType::EdgeAccessConstPosPerm);
      },
      [&](ScAddr const & addr) -> bool
      {
        return !m_ctx->HelperCheckEdge(structureAddr, addr, ScType::EdgeAccessConstPosPerm);
      });
  EXPECT_EQ(count, 0u);
}

TEST_F(ScTemplateSearchApiTest, SearchVarTriple)
{
  ScTemplate templ;
  templ.Triple(ScType::Unknown >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  EXPECT_THROW(
      m_ctx->HelperSearchTemplate(
          templ,
          [&](ScTemplateSearchResultItem const &)
          {
            ++count;
          }),
      utils::ExceptionInvalidState);

  EXPECT_EQ(count, 0u);
}

TEST_F(ScTemplateSearchApiTest, SearchEmpty)
{
  ScTemplate templ;
  size_t count = 0;
  m_ctx->HelperSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const &)
      {
        ++count;
      });

  EXPECT_EQ(count, 0u);
}

TEST_F(ScTemplateSearchApiTest, SearchWithStopRequest)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge1 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(edge1.IsValid());
  ScAddr const addr3 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge2 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr3);
  EXPECT_TRUE(edge2.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  m_ctx->HelperSmartSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest
      {
        ScAddr foundAddr;

        EXPECT_TRUE(item.Get(0, foundAddr));
        EXPECT_EQ(foundAddr, addr1);
        EXPECT_TRUE(item.Get("_addr1", foundAddr));
        EXPECT_EQ(foundAddr, addr1);

        EXPECT_TRUE(item.Get(1, foundAddr));
        EXPECT_TRUE(item.Get("_edge", foundAddr));

        EXPECT_TRUE(item.Get(2, foundAddr));
        EXPECT_TRUE(item.Get("_addr2", foundAddr));

        ++count;
        return ScTemplateSearchRequest::STOP;
      });

  EXPECT_EQ(count, 1u);
}

TEST_F(ScTemplateSearchApiTest, SearchWithErrorRequest)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge1 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(edge1.IsValid());
  ScAddr const addr3 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge2 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr3);
  EXPECT_TRUE(edge2.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  EXPECT_THROW(
      m_ctx->HelperSmartSearchTemplate(
          templ,
          [&](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest
          {
            ScAddr foundAddr;

            EXPECT_TRUE(item.Get(0, foundAddr));
            EXPECT_EQ(foundAddr, addr1);
            EXPECT_TRUE(item.Get("_addr1", foundAddr));
            EXPECT_EQ(foundAddr, addr1);

            EXPECT_TRUE(item.Get(1, foundAddr));
            EXPECT_TRUE(item.Get("_edge", foundAddr));

            EXPECT_TRUE(item.Get(2, foundAddr));
            EXPECT_TRUE(item.Get("_addr2", foundAddr));

            ++count;
            return ScTemplateSearchRequest::ERROR;
          }),
      utils::ExceptionInvalidState);

  EXPECT_EQ(count, 1u);
}

TEST_F(ScTemplateSearchApiTest, SearchWithContinueRequest)
{
  ScAddr const addr1 = m_ctx->GenerateNode(ScType::NodeConst);
  EXPECT_TRUE(addr1.IsValid());
  ScAddr const addr2 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge1 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr2);
  EXPECT_TRUE(edge1.IsValid());
  ScAddr const addr3 = m_ctx->GenerateNode(ScType::NodeConstAbstract);
  EXPECT_TRUE(addr2.IsValid());
  ScAddr const edge2 = m_ctx->GenerateConnector(ScType::EdgeAccessConstPosPerm, addr1, addr3);
  EXPECT_TRUE(edge2.IsValid());

  ScTemplate templ;
  templ.Triple(addr1 >> "_addr1", ScType::EdgeAccessVarPosPerm >> "_edge", ScType::Unknown >> "_addr2");

  size_t count = 0;
  m_ctx->HelperSmartSearchTemplate(
      templ,
      [&](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest
      {
        ScAddr foundAddr;

        EXPECT_TRUE(item.Get(0, foundAddr));
        EXPECT_EQ(foundAddr, addr1);
        EXPECT_TRUE(item.Get("_addr1", foundAddr));
        EXPECT_EQ(foundAddr, addr1);

        EXPECT_TRUE(item.Get(1, foundAddr));
        EXPECT_TRUE(item.Get("_edge", foundAddr));

        EXPECT_TRUE(item.Get(2, foundAddr));
        EXPECT_TRUE(item.Get("_addr2", foundAddr));

        ++count;
        return ScTemplateSearchRequest::CONTINUE;
      });

  EXPECT_EQ(count, 2u);
}

TEST_F(ScTemplateSearchApiTest, SearchTemplateWithConstantTriple)
{
  ScAddr const & nodeAddr1 = m_ctx->GenerateNode(ScType::NodeConst);
  ScAddr const & nodeAddr2 = m_ctx->GenerateNode(ScType::NodeConst);
  ScAddr const & edgeAddr = m_ctx->GenerateConnector(ScType::EdgeDCommonConst, nodeAddr1, nodeAddr2);
  m_ctx->GenerateConnector(ScType::EdgeAccessConstPosTemp, nodeAddr2, edgeAddr);

  ScTemplate templ;
  templ.Triple(nodeAddr1, edgeAddr, nodeAddr2);
  templ.Triple(nodeAddr2, ScType::EdgeAccessVarPosTemp, edgeAddr);

  ScTemplateSearchResult result;
  EXPECT_TRUE(m_ctx->HelperSearchTemplate(templ, result));

  EXPECT_EQ(result[0][nodeAddr1], nodeAddr1);
  EXPECT_EQ(result[0][edgeAddr], edgeAddr);
  EXPECT_EQ(result[0][nodeAddr2], nodeAddr2);

  for (ScAddr const & addr : result[0])
    EXPECT_TRUE(m_ctx->IsElement(addr));
}
