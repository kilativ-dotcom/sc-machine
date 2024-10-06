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

using GWFTranslatorTest = ScBuilderTest;

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

#define BASE_TEST_PATH SC_BUILDER_KB "/tests-gwf-to-scs/"

std::string RemoveEmptyLines(std::string const & input)
{
  std::stringstream ss(input);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(ss, line))
  {
    line.erase(
        line.begin(),
        std::find_if(
            line.begin(),
            line.end(),
            [](unsigned char ch)
            {
              return !std::isspace(ch);
            }));
    line.erase(
        std::find_if(
            line.rbegin(),
            line.rend(),
            [](unsigned char ch)
            {
              return !std::isspace(ch);
            })
            .base(),
        line.end());

    if (!line.empty())
    {
      lines.push_back(line);
    }
  }

  std::stringstream result;
  for (auto const & l : lines)
  {
    result << l << "\n";
  }

  return result.str();
}

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
    return RemoveEmptyLines(buffer.str());
  }
  catch (utils::ScException const & e)
  {
    SC_LOG_ERROR("Exeption in reading file to string " + std::string(e.what()));
    return "";
  }
}

std::pair<std::shared_ptr<SCsTree>, std::shared_ptr<SCsTree>> CompareSCsFiles(
    std::string const & fileName,
    GWFTranslator translator)
{
  std::string const gwfFilePath = BASE_TEST_PATH + fileName;
  std::string const scsFilePath = gwfFilePath + ".scs";

  std::string const gwfStr = translator.XmlFileToString(gwfFilePath);
  std::string const scsStr = translator.GWFToScs(gwfStr, BASE_TEST_PATH);

  std::string const exampleScs = RemoveEmptyLines(ReadFileToString(scsFilePath));

  auto const exampleTree = SCsTree::ParseTree(exampleScs);
  auto const resultTree = SCsTree::ParseTree(scsStr);

  return std::make_pair(exampleTree, resultTree);
}

TEST_F(GWFTranslatorTest, EmptyFile)
{
  GWFTranslator translator(*m_ctx);

  std::string const filePath = BASE_TEST_PATH "empty_file.gwf";

  EXPECT_THROW(translator.XmlFileToString(filePath), utils::ExceptionParseError);
}

TEST_F(GWFTranslatorTest, EmptyStatic)
{
  GWFTranslator translator(*m_ctx);

  std::string const filePath = BASE_TEST_PATH "empty_static.gwf";

  std::string const gwfStr = translator.XmlFileToString(filePath);
  EXPECT_THROW(translator.GWFToScs(gwfStr, BASE_TEST_PATH), utils::ExceptionParseError);
}

TEST_F(GWFTranslatorTest, EmptyContour)
{
  GWFTranslator translator(*m_ctx);

  auto const & trees = CompareSCsFiles("empty_contour.gwf", translator);
  bool const diff = SCsTree::CompareTrees(trees)->empty();

  EXPECT_TRUE(diff);
}

TEST_F(GWFTranslatorTest, LotOfContours)
{
  GWFTranslator translator(*m_ctx);

  auto const & trees = CompareSCsFiles("lot_of_contours.gwf", translator);
  bool const diff = SCsTree::CompareTrees(trees)->empty();

  EXPECT_TRUE(diff);
}

TEST_F(GWFTranslatorTest, ContentTypes)
{
  GWFTranslator translator(*m_ctx);

  auto const & trees = CompareSCsFiles("content_types.gwf", translator);
  bool const diff = SCsTree::CompareTrees(trees)->empty();

  EXPECT_TRUE(diff);

  EXPECT_TRUE(CompareFiles(BASE_TEST_PATH "ostis.png", BASE_TEST_PATH "ostis_ref.png"));
  std::filesystem::remove(BASE_TEST_PATH "ostis.png");
}
