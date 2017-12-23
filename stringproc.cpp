#include "stringproc.h"

#include <string.h>

void lib::pad(std::string& str)
{
  char* ptr = (char*) str.c_str();
  char* bptr = ptr;
  char* eptr = ptr + ( ( str.size() - 1 ) * sizeof(char));
  while(isspace(*eptr) && eptr != ptr) eptr--;
  while(isspace(*ptr) && *ptr != '\0') ptr++;

  str = str.substr(( ptr - bptr ) / sizeof(char), 1 + ( eptr - ptr ) / sizeof(char));
}

long lib::begins_with(std::string& str, std::string query)
{
  if(strncmp(str.c_str(), query.c_str(), query.size()) == 0)
    return query.size();
  return -1;
}

long lib::ends_with(std::string& str, std::string query)
{
  if(strncmp(str.c_str() + ( str.size() * sizeof(char) - query.size() * sizeof(char) ), query.c_str(), query.size()) == 0)
    return str.size() - query.size() - 1;
  return -1;
}

long lib::find_next(std::string& str, char ch, long pos)
{
  for(size_t i = pos; i < str.size(); i++)
  {
    if(ch == str[i])
      return i;
  }
  return -1;
}
