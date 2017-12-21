#ifndef _STRINGPROC_H
#define _STRINGPROC_H

#include <string>
namespace lib
{
  void pad(std::string& str);
  long begins_with(std::string& str, std::string query);
  long ends_with(std::string& str, std::string query);
  long find_next(std::string& str, char ch, long start_pos);
}

#endif