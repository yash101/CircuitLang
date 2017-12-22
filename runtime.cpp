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

std::string read_file(std::string location);



/*

What I will need:

- A function table - a table with the code for each function to execute
- A data table - store the data passed between functions; allows for garbage collection
- An execution queue

*/

CircuitLangCTX::CircuitLangCTX(std::string str, bool file)
{
  if(file)
  {
    program_file = str;
  }
  else
  {
    program_code = str;
  }
}





/* TODO: complete! */
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
}

/* Reads a file into a string */
std::string read_file(std::string location, int* error)
{
  FILE* fil = fopen(location.c_str(), "r");
  if(!fil)
  {
    if(error)
      *error = ERR_FOPEN;
    return "";
  }
  FileCloser fp(fil);     // Don't forget to use a condom!

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
  AutoPointer<char> bptr(buffer, true);   // Again, condoms can be a lifesaver
}






int process_file(const char* filename, std::vector<std::string>& lines, std::map<std::string, bool>& complete)
{
  if(complete[std::string(filename)] == true) return SUCCESS;
  complete[std::string(filename)] = true;
  FILE* file = fopen(filename, "r");
  FileCloser fprotect(file);
  if(!file)
    return ERR_FOPEN;

  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = new char[len + 1];
  if(!buffer)
  {
    return ERR_MEM_ALLOC;
  }

  fread(buffer, sizeof(char), len, file);
  buffer[len] = '\0';
}