#include "runtime.h"
#include <sstream>

using namespace runtime::datatypes;

/* DataType - should not be used for anything; may result in undefined behaviour */

std::string DataType::toString()
{
  return std::string("undefined");
}

bool DataType::isArray()
{
  return array;
}

int DataType::getDataType()
{
  return Undefined;
}

/* NullType - used to indicate a NULL in code; default initializer */

std::string NullType::toString()
{
  return std::string("null");
}

int NullType::getDataType()
{
  return Null;
}

/* StringType - used to hold a String */

std::string StringType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << '\"' << data[i] << "\", ";
    }
    ss << ", \"" << data.back() << "\"]";

    return ss.str();
  }
  return "\"" + data.front() + "\"";
}

int StringType::getDataType()
{
  return String;
}

/* BoolType - used to hold booleans */
std::string BoolType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << '\"' << (data[i] ? "true" : "false") << ", ";
    }
    ss << ", " << (data.back() ? "true" : "false") << "]";

    return ss.str();
  }
  return data.front() ? "true" : "false";
}

int BoolType::getDataType()
{
  return Bool;
}

/* CharType */
std::string CharType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "'" << data[i] << "', ";
    }
    ss << ", '" << data.back() << "']";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int CharType::getDataType()
{
  return Char;
}

/* ShortType */

std::string ShortType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int ShortType::getDataType()
{
  return Short;
}

/* IntType */

std::string IntType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int IntType::getDataType()
{
  return Int;
}

/* LongType */

std::string LongType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int LongType::getDataType()
{
  return Long;
}

/* FloatType */

std::string FloatType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int FloatType::getDataType()
{
  return Int;
}

/* DoubleType */

std::string DoubleType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int DoubleType::getDataType()
{
  return Int;
}

/* UnsignedCharType */

std::string UnsignedCharType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int UnsignedCharType::getDataType()
{
  return Int;
}

/* UnsignedShortType */

std::string UnsignedShortType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int UnsignedShortType::getDataType()
{
  return Int;
}

/* UnsignedIntType */

std::string UnsignedIntType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int UnsignedIntType::getDataType()
{
  return Int;
}

/* UnsignedLongType */

std::string UnsignedLongType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i] << ", ";
    }
    ss << ", " << data.back() << "]";

    return ss.str();
  }
  return std::string("") + (char) data.front();
}

int UnsignedLongType::getDataType()
{
  return Int;
}

/* StructureType */

std::string StructureType::toString()
{
  std::stringstream ss;
  if(array)
  {
    if(data.empty())
      return "[]";

    ss << "[";
    // Don't go till the end
    for(size_t i = 0; i < data.size() - 1; i++)
    {
      ss << "" << data[i].toString() << ", ";
    }
    ss << ", " << data.back().toString() << "]";

    return ss.str();
  }
  return data.front().toString();
}

int StructureType::getDataType()
{
  return Structure;
}
