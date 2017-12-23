#include "runtime.h"
#include "errors.h"
#include "stringproc.h"
#include "protection.h"
#include "config.h"

#include <stdio.h>

#include <vector>
#include <string>
#include <map>
#include <string.h>
#include <sstream>

// Less typing can be better :)
using namespace std;
using namespace runtime;

std::string read_file(std::string location, int* error);



/*

What I will need:

- A function table - a table with the code for each function to execute
- A data table - store the data passed between functions; allows for garbage collection
- An execution queue

*/



CircuitLangCTX::CircuitLangCTX(std::string str, bool file) :
  program_counter(0)
{
  if(file)
  {
    program_file = str;
    int err;
    program_code = read_file(program_file, &err);
    if(err != SUCCESS)
      throw std::exception("Error: could not read file", err);
  }
  else
  {
    program_code = str;
  }
}

CircuitLangCTX::CircuitLangCTX() :
  program_counter(0)
{
}

int CircuitLangCTX::LoadFromFile(std::string file)
{
  program_file = file;
  int err;
  program_code = read_file(file, &err);
  return err;
}

int CircuitLangCTX::LoadFromCodeString(std::string code)
{
  program_code = code;
  return SUCCESS;
}

int CircuitLangCTX::Run()
{
  if(program_code.empty())
    return SUCCESS;

  int ret;
//  if(( ret = ParseProgram() ) != SUCCESS)
    return ret;
}

/* TODO: complete! This function just appends reference and path currently but needs to be completely written to be smarter */
std::string runtime::resolve_file_location(std::string reference, std::string path)
{
  // Do nothing if we have an absolute URL
  if(path.front() != '/')
  {
    std::vector<std::string> parts;
    std::stringstream str(reference);
    std::string buf;
    while(std::getline(str, buf, '/'))
    {
      parts.push_back(buf);
    }
  }

  if(reference.back() == '/')
    reference.pop_back();
  if(path.front() == '/')
    return reference + path;
  else
    return reference + '/' + path;
}

/* Reads a file into a string */
std::string read_file(std::string location, int* error)
{
  if(error)
    *error = SUCCESS;

  FILE* fil = fopen(location.c_str(), "r");
  if(!fil)
  {
    if(error)
      *error = ERR_FOPEN;
    return "";
  }
  FileCloser fp(fil);     // Protection

  fseek(fil, 0, SEEK_END);
  long l = ftell(fil);
  fseek(fil, 0, SEEK_SET);

  char* buffer = new char[l + 1];
  if(!buffer)
  {
    if(error)
      *error = ERR_MEM_ALLOC;
    return "";
  }
  AutoPointer<char> bptr(buffer, true);   // Protection

  int read = fread(buffer, sizeof(char), l, fil);
  buffer[l] = '\0';

  return std::string(buffer, read + 1);
}