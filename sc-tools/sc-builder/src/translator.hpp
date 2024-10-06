/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include <sc-memory/sc_addr.hpp>

class Translator
{
public:
  struct Params
  {
    //! Name of file to translate
    std::string m_fileName;
    //! Flag to generate format information based on file extensions
    bool m_autoFormatInfo;
    //! output structure
    ScAddr m_outputStructure;
  };

  explicit Translator(class ScMemoryContext & context);
  virtual ~Translator() = default;

  /*! Translate specified file into memory
   * @param params Input parameters
   * @return If file translated without any errors, then returns true; otherwise returns false.
   */
  bool Translate(Params const & params);

  //! Implementation of translate
  virtual bool TranslateImpl(Params const & params) = 0;

  static void Clean(ScMemoryContext & ctx);

protected:
  static void GetFileContent(std::string const & fileName, std::string & outContent);

  //! Pointer to memory context
  class ScMemoryContext & m_ctx;
};
