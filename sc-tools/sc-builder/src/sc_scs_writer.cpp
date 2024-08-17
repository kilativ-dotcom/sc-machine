#include "sc_scs_writer.hpp"

// BUFFER CLASS

Buffer::Buffer()
  : value("")
{
}

void Buffer::Write(std::string const & s)
{
  value += s;
}

void Buffer::AddTabs(std::size_t count)
{
  std::istringstream iss(value);
  std::string line;
  std::string new_value;
  std::string tabs(count * 4, ' ');

  while (std::getline(iss, line))
  {
    new_value += tabs + line + "\n";
  }

  value = new_value;
}

std::string Buffer::GetValue() const
{
  return value;
}

// UTILS CLASS

std::string Utils::ReplaceAll(std::string const & str, std::string const & from, std::string const & to)
{
  std::string result = str;
  size_t start_pos = 0;
  while ((start_pos = result.find(from, start_pos)) != std::string::npos)
  {
    result.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return result;
}

std::string Utils::MakeAlias(std::string const & prefix, std::string const & element_id)
{
  return "@" + prefix + "_" + ReplaceAll(element_id, "-", "_");
}

bool Utils::IsVariable(std::string const & el_type)
{
  return el_type.find("/var/") != std::string::npos;
}

// SCS WRITER CLASS

const std::unordered_map<std::string, std::string> IMAGE_FORMATS = {{".png", "format_png"}};

const std::string N_REL_SYSTEM_IDTF = "nrel_system_identifier";
const std::string N_REL_MAIN_IDTF = "nrel_main_idtf";

std::string SCSWriter::Write(
    std::unordered_map<std::string, std::shared_ptr<SCGElement>> const & elements,
    std::string const & filePath)
{
  Buffer buffer;

  for (auto const & el : elements)
  {
    CorrectIdtf(buffer, el.second);
  }

  ProcessElementsList(elements, buffer, filePath);

  return buffer.GetValue();
}

void SCSWriter::ProcessElementsList(
    std::unordered_map<std::string, std::shared_ptr<SCGElement>> const & elementsList,
    Buffer & buffer,
    std::string const & filePath)
{
  try
  {
    for (auto & element : elementsList)
    {
      auto const tag = element.second->getTag();
      if (tag == "node" || tag == "bus")
      {
        WriteNode(buffer, element.second, filePath);
        continue;
      }

      auto const parent = element.second->getParent();
      if (parent == "0")
      {
        if (tag == "pair" || tag == "arc")
        {
          std::shared_ptr<Connector> connector = std::dynamic_pointer_cast<Connector>(element.second);
          WriteConnector(buffer, connector);
        }
        else if (tag == "contour")
        {
          std::shared_ptr<Contour> contour = std::dynamic_pointer_cast<Contour>(element.second);
          WriteContour(buffer, contour, 1);
        }
      }
    }
  }
  catch (utils::ScException const & e)
  {
    SC_LOG_ERROR("SCSWriter: exception in process elements " + std::string(e.what()));
  }
}

void SCSWriter::WriteNode(Buffer & buffer, std::shared_ptr<SCGElement> const & element, std::string const & filePath)
{
  std::shared_ptr<Link> link = std::dynamic_pointer_cast<Link>(element);

  if (link->getContentType() != "0")
  {
    WriteLink(buffer, link, filePath);
  }
  else
  {
    const std::string nodeType = element->getType();
    std::string scsNodeType = "";

    SCGToSCSElement::ConvertNodeType(element->getType(), scsNodeType);

    if (scsNodeType.empty())
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "No matching scs node type for scg node: " + nodeType);
    }

    buffer.Write(element->getIdtf() + "\n  <- " + scsNodeType + ";;\n\n");
  }
}

void SCSWriter::WriteConnector(Buffer & buffer, std::shared_ptr<Connector> const & connector)
{
  const std::string edgeType = connector->getType();
  std::string symbol = "";
  
  bool isUnsupported = SCGToSCSElement::ConvertEdgeType(edgeType, symbol);

  if (symbol.empty())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "No matching scs node type for scg node: " + edgeType);
  }

  const std::string alias = Utils::MakeAlias("edge", connector->getId());
  if (isUnsupported)
  {
    buffer.Write(
        alias + " = (" + connector->getSource()->getIdtf() + " => " + connector->getTarget()->getIdtf() + ");;\n");
    buffer.Write(symbol + " -> " + alias + ";;\n");
  }
  else
  {
    buffer.Write(
        alias + " = (" + connector->getSource()->getIdtf() + " " + symbol + " " + connector->getTarget()->getIdtf()
        + ");;\n");
  }

  buffer.Write("\n");
}

void SCSWriter::WriteContour(Buffer & buffer, std::shared_ptr<Contour> const & contour, int tabLevel)
{
  Buffer contourBuffer;

  auto contourElements = contour->getElements();

  for (auto element : contourElements)
  {
    auto const tag = element->getTag();

    if (tag == "node" || tag == "bus")
    {
      WriteNodeForContour(buffer, element, contour);
    }
    else if (tag == "pair" || tag == "arc")
    {
      std::shared_ptr<Connector> connector = std::dynamic_pointer_cast<Connector>(element);

      if (connector->getSource() == contour)
      {
        if (CheckForNode(connector->getTarget(), contourElements))
        {
          continue;
        }
      }

      WriteConnector(contourBuffer, connector);
    }
    else if (tag == "contour")
    {
      std::shared_ptr<Contour> contour = std::dynamic_pointer_cast<Contour>(element);
      WriteContour(contourBuffer, contour, tabLevel + 1);
    }
    else
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Uncorrect element tag " + tag);
    }
  }
  contourBuffer.AddTabs(tabLevel);
  buffer.Write(contour->getIdtf() + " = [*\n" + contourBuffer.GetValue() + "*];;\n");
}

void SCSWriter::WriteLink(Buffer & buffer, std::shared_ptr<Link> const & link, std::string const & filePath)
{
  int const contentType = std::stoi(link->getContentType());

  bool isUrl = false;
  bool isImage = false;
  std::string contentRes;
  std::string imageFormat;

  std::string fileName = link->getFileName();
  std::string contentData = link->getContentData();

  std::filesystem::path basePath = std::filesystem::path(filePath).parent_path();
  std::filesystem::path fullPath = basePath / fileName;

  switch (contentType)
  {
  case 1:
    contentRes = contentData;
    break;
  case 2:
    contentRes = "\"int64:" + contentData + "\"";
    break;
  case 3:
    contentRes = "\"float:" + contentData + "\"";
    break;
  case 4:
  {
    std::ofstream file(fullPath, std::ios::binary);
    if (!file)
    {
      SC_THROW_EXCEPTION(utils::ExceptionInvalidParams, "Failed to open file for writing: " + fullPath.string());
    }
    file.write(contentData.data(), contentData.size());
    file.close();

    contentRes = "file://" + fullPath.filename().string();
    isUrl = true;

    std::string fileExtension = fullPath.extension().string();
    auto it = IMAGE_FORMATS.find(fileExtension);
    if (it != IMAGE_FORMATS.end())
    {
      imageFormat = it->second;
      isImage = true;
    }
    else
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "File extension not found: " + fileExtension);
    }
    break;
  }
  default:
    SC_THROW_EXCEPTION(utils::ExceptionInvalidType, "Content type not supported: " + link->getId());
    contentRes = "?Content type is not supported?";
  }

  auto const elIdtf = link->getIdtf();

  if (isUrl)
  {
    buffer.Write(elIdtf + " = \"" + contentRes + "\";;\n");
    if (isImage)
    {
      buffer.Write("@format_edge = (" + elIdtf + " => " + imageFormat + ");;\n");
      buffer.Write("@nrel_format_edge = (nrel_format -> @format_edge);;\n");
    }
  }
  else
  {
    std::string isVar = Utils::IsVariable(link->getType()) ? "_" : "";
    buffer.Write(elIdtf + " = " + isVar + "[" + contentRes + "];;\n");
  }

  buffer.Write("\n");
}

void SCSWriter::WriteNodeForContour(
    Buffer & buffer,
    std::shared_ptr<SCGElement> const & node,
    std::shared_ptr<Contour> const & contour)
{
  size_t counter = 1;
  auto & elements = contour->getElements();

  elements.erase(
      std::remove_if(
          elements.begin(),
          elements.end(),
          [&](std::shared_ptr<SCGElement> & element)
          {
            auto connector = std::dynamic_pointer_cast<Connector>(element);
            if (connector && connector->getSource() == contour && connector->getTarget() == node)
            {
              counter++;
              return true;
            }
            return false;
          }),
      elements.end());

  auto const edgeName =
      ("@edge_from_contour_" + contour->getId() + "_to_node_" + node->getId() + "_" + std::to_string(counter));
  buffer.Write(edgeName + " = (" + contour->getIdtf() + " -> " + node->getIdtf() + ");;\n\n");
}

void SCSWriter::CorrectIdtf(Buffer & buffer, std::shared_ptr<SCGElement> const & element)
{
  std::regex idtfPatternEng("^[0-9a-zA-Z_]*$");
  std::regex idtfPatternRus("^[0-9a-zA-Z_\\xD0\\x80-\\xD1\\x8F\\xD1\\x90-\\xD1\\x8F\\xD1\\x91\\xD0\\x81*' ]*$");

  std::string systemIdtf = element->getIdtf();
  bool isVar = Utils::IsVariable(element->getType());

  std::string mainIdtf;
  if (!std::regex_match(systemIdtf, idtfPatternEng))
  {
    if (std::regex_match(systemIdtf, idtfPatternRus))
    {
      mainIdtf = systemIdtf;
    }
    systemIdtf.clear();
  }

  if (systemIdtf.empty())
  {
    if (isVar)
    {
      element->setIdtf(".._el_" + Utils::ReplaceAll(element->getId(), "-", "_"));
    }
    else
    {
      element->setIdtf("..el_" + Utils::ReplaceAll(element->getId(), "-", "_"));
    }
  }
  else if (isVar)
  {
    if (!systemIdtf.empty() && systemIdtf[0] != '_')
    {
      element->setIdtf("_" + Utils::ReplaceAll(systemIdtf, "-", "_"));
    }
  }
  else
  {
    if (!systemIdtf.empty() && systemIdtf[0] == '_')
    {
      element->setIdtf(systemIdtf.substr(1));
      element->setIdtf(Utils::ReplaceAll(element->getIdtf(), "-", "_"));
    }
  }

  if (!mainIdtf.empty())
  {
    std::string output;
    if (mainIdtf[0] == '[')
    {
      output = "\n" + element->getIdtf() + "\n => " + N_REL_MAIN_IDTF + ": " + mainIdtf + ";;\n";
    }
    else
    {
      output = "\n" + element->getIdtf() + "\n => " + N_REL_MAIN_IDTF + ": [" + mainIdtf + "];;\n";
    }
    buffer.Write(output + "\n");
  }

  if (element->getTag() == "pair" || element->getTag() == "arc")
  {
    auto id = element->getId();
    element->setIdtf(Utils::MakeAlias("edge", id));
  }
}

bool SCSWriter::CheckForNode(
    std::shared_ptr<SCGElement> refElement,
    std::vector<std::shared_ptr<SCGElement>> contourElements)
{
  for (auto const & element : contourElements)
  {
    if (element == refElement)
    {
      return true;
    }
  }

  return false;
}
