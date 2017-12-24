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
#include <stack>

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
  if(( ret = ParseProgram() ) != SUCCESS)
    return ret;

  return SUCCESS;
}

/*
This function parses the program code; it determines syntax errors and converts
  the code into the executable format
*/
int CircuitLangCTX::ParseProgram()
{
  // The program code shall be contained in program_code by now

  // Functions shall start with a forward slash
  // Global definitions and imports shall start with an `@`

  CheckCommonSyntaxErrors();

  // This vector holds each line of code
  std::vector<std::string> lines;
  {
  // This stringstream holds the code, makes string processing much easiers
    std::stringstream buffer(program_code);
    std::string tmp;

    // Break into lines
    while(std::getline(buffer, tmp))
    {
      // tmp has a line
      // pad the line
      lib::pad(tmp);
      // add to the line vector
      lines.push_back(tmp);
    }
  }

  std::vector<CL_function_instruction_container> instructions;

  size_t skip_lines = 0;
  // Process the lines (of code)
  // Note that we are doing this line-by-line so that we can give better error messages
  for(size_t i = 0; i < lines.size(); i += skip_lines)
  {
    // Skip processing empty lines
    if(lines[i].empty())
      continue;

    /* TODO: support multi-line statements */

    // Process functions:
    // - /<function name>[ ]([ <arg0> , <arg1> , <arg...> ])
    // Functions begin with a forward slash
    // Global definitions and imports begin with an `@`
    if(lines[i].front() == '/')
    {
      // Let's begin a function
      // Iterate until end or parenthesis (if end reached, error!)
      size_t pos = 1;
      while(pos < lines[i].size() && lines[i][pos] != '(')
        pos++;
      if(pos == lines[i].size())
      {
        fprintf(stderr, "SyntaxError: %s:%d expected parameter list for function\n\t%s", program_file.c_str(), i, lines[i].c_str());
        return ERR_PARSE_FAILED;
      }

      // Pos contains the location of the first parenthesis
      // Pos-1 marks the last possible location for a function name; thus we can
      // pad a string from lines[i][1] to lines[i][pos-1] for the function name
      std::string fname = lines[i].substr(1, pos - 1 - 1);

      // Remove before/after space from the function name
      lib::pad(fname);

      // Add the function
      CL_function_instruction_container v;
      v.function_name = fname;
      instructions.push_back(v);
    }

    // Process lines beginning with '@' - global definitions and imports
    // - @define <variable name>[ = <initializer>];
    // - @import <filename>;

    else if(lines[i].front() == '@')
    {
    }

    // Process each of the possible operations
    // - var[ ]<name>[ ]string [= <initializer];
    // - <name>[ ]<operator>;    i++, j--, i *= 2, etc.
    // - <name>[ ]=[ ][<in> <operator> <in> <...>];   i = 1 + 2 * 3, j = 2, etc.
    // - >[ ]<name>[ ]( [<arg0> , <arg1> , <...>] );    >func(1, 2, 3)
    else
    {
    }
  }

  return SUCCESS;
}

int CircuitLangCTX::CheckCommonSyntaxErrors()
{
}

// Check if a letter is a valid first letter in a function name
bool CircuitLangCTX::ValidFirstLetterInFunctionName(char ch)
{
  return isalpha(ch) || ch == '_';
}

// Check if a letter is allowed in a function name
bool CircuitLangCTX::ValidLetterInFunctionName(char ch)
{
  return isalnum(ch) || ch == '_';
}

bool CircuitLangCTX::CheckFunctionName(std::string name)
{
  // Empty names aren't allowed
  if(name.empty())
    return false;
  // Check the first letter
  if(!ValidFirstLetterInFunctionName(name.front()))
    return false;

  // Check the rest of the letters
  for(size_t i = 1; i < name.size(); i++)
  {
    if(!ValidLetterInFunctionName(name[i]))
      return false;
  }
}

/*
TODO: complete! This function just appends reference and path currently but needs
to be completely written to be smarter
*/
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