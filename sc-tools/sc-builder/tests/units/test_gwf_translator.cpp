/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>

#include "builder_test.hpp"

#include <filesystem>

#include "sc-memory/sc_utils.hpp"
#include "../../src/gwf_translator.hpp"
#include "../../src/sc_scs_tree.hpp"

#define BASE_TEST_PATH "/home/iromanchuk/sc-machine/sc-tools/sc-builder/tests/kb/tests-gwf-to-scs/"

using GWFTranslatorTest = ScBuilderTest;

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

std::vector<char> ReadFileToBytes(std::string const & filePath)
{
  std::ifstream file(filePath, std::ios::binary | std::ios::ate);
  if (!file)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Could not open file: " + filePath);
  }
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size))
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Could not read file: " + filePath);
  }
  return buffer;
}

bool CompareFiles(std::string const & filePath1, std::string const & filePath2)
{
  try
  {
    std::vector<char> file1Bytes = ReadFileToBytes(filePath1);
    std::vector<char> file2Bytes = ReadFileToBytes(filePath2);
    return file1Bytes == file2Bytes;
  }
  catch (utils::ScException const & e)
  {
    SC_LOG_ERROR("Exeption in reading file to string " + std::string(e.what()));
    return false;
  }
}

std::string ReadFileToString(std::string const & filePath)
{
  try
  {
    std::ifstream file(filePath);
    if (!file.is_open())
    {
      SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Could not open file");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }
  catch (utils::ScException const & e)
  {
    SC_LOG_ERROR("Exeption in reading file to string " + std::string(e.what()));
    return nullptr;
  }
}

TEST_F(GWFTranslatorTest, EmptyFile)
{
  GWFTranslator translator(*m_ctx);

  const std::string filePath = BASE_TEST_PATH "empty_file.gwf";

  EXPECT_THROW(translator.XmlFileToString(filePath), utils::ExceptionParseError);
}

TEST_F(GWFTranslatorTest, EmptyStatic)
{
  GWFTranslator translator(*m_ctx);

  const std::string filePath = BASE_TEST_PATH "empty_static.gwf";

  const std::string gwfStr = translator.XmlFileToString(filePath);
  EXPECT_THROW(translator.GwfToScs(gwfStr, BASE_TEST_PATH), utils::ExceptionParseError);
}

TEST_F(GWFTranslatorTest, EmptyContour)
{
  GWFTranslator translator(*m_ctx);

  const std::string filePath = BASE_TEST_PATH "empty_contour.gwf";

  const std::string gwfStr = translator.XmlFileToString(filePath);
  const std::string scsStr = translator.GwfToScs(gwfStr, BASE_TEST_PATH);

  const std::string exampleScs = ReadFileToString(BASE_TEST_PATH "empty_contour.gwf.scs");

  auto const exampleTree = ScsTree::ParseTree(exampleScs);
  auto const resultTree = ScsTree::ParseTree(scsStr);

  EXPECT_FALSE(ScsTree::CompareTrees(exampleTree, resultTree));
}

TEST_F(GWFTranslatorTest, LotOfContours)
{
  GWFTranslator translator(*m_ctx);

  const std::string filePath = BASE_TEST_PATH "lot_of_contours.gwf";

  const std::string gwfStr = translator.XmlFileToString(filePath);
  const std::string scsStr = translator.GwfToScs(gwfStr, BASE_TEST_PATH);

  const std::string exampleScs = ReadFileToString(BASE_TEST_PATH "lot_of_contours.gwf.scs");

  auto const exampleTree = ScsTree::ParseTree(exampleScs);
  auto const resultTree = ScsTree::ParseTree(scsStr);

  EXPECT_FALSE(ScsTree::CompareTrees(exampleTree, resultTree));
}

TEST_F(GWFTranslatorTest, ContentTypes)
{
  GWFTranslator translator(*m_ctx);

  const std::string filePath = BASE_TEST_PATH "content_types.gwf";

  const std::string gwfStr = translator.XmlFileToString(filePath);
  const std::string scsStr = translator.GwfToScs(gwfStr, BASE_TEST_PATH);

  const std::string exampleScs = ReadFileToString(BASE_TEST_PATH "content_types.gwf.scs");

  auto const exampleTree = ScsTree::ParseTree(exampleScs);
  auto const resultTree = ScsTree::ParseTree(scsStr);

  EXPECT_TRUE(CompareFiles(BASE_TEST_PATH "ostis.png", BASE_TEST_PATH "ostis_ref.png"));

  std::filesystem::remove(BASE_TEST_PATH "ostis.png");

  EXPECT_FALSE(ScsTree::CompareTrees(exampleTree, resultTree));
}