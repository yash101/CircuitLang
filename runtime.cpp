#include "runtime.h"
#include "errors.h"
#include "stringproc.h"
#include "protection.h"

#include <stdio.h>

#include <vector>
#include <string>
#include <map>
#include <string.h>
#include <sstream>


/*

What I will need:

- A function table - a table with the code for each function to execute
- A data table - store the data passed between functions; allows for garbage collection
- An execution queue

*/

int run_program(const char* filename);
int process_file(const char* filename, std::vector<std::string>& lines, std::map<std::string, bool>& complete);

int runtime::run_program(const char* filename)
{
  std::vector<std::string> lines;
  std::map<std::string, bool> files_done;

  process_file(filename, lines, files_done);

  return SUCCESS;
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