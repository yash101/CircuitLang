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

  // CheckCommonSyntaxErrors();   // Commented out for now because it's not implemented

  // The string will contain the line; the size_t will contain the line
  //  number of the first line of code

//  std::vector<CL_string_sizet_tuple> instruction_lines; *********************

  std::stringstream code(program_code);

  // Which line number are we on?
  size_t curpos = 0;

  // This loop shall process all the program code
  while(true)
  {
    // Holds the next line of code
    size_t nextpos = curpos;

    // Temporary string for usage
    std::string buffer;

    // This macro definition will help us out!
#define GETLINE(fil, buf) { std::getline(fil, buf); nextpos++; }

    GETLINE(code, buffer);
    lib::pad(buffer);

    // Remove comments
    size_t cpos = buffer.find("#");
    // Comment found
    if(cpos != std::string::npos)
    {
      if(cpos == 0)
      {
        // Line is a comment
        continue;
      }
      // Comment is at the end of line
      else if(buffer[cpos - 1] != '\\')
      {
      }
    }

    // We need to decide what this line of code is.
    // If it begins with a '/', it is a function definition

    if(buffer.front() == '/') // It is a function?
    {
      // Read until we reach the closing parenthesis
      size_t pos = std::string::npos;

      // Loop until we have a closing parenthesis
      while(pos == std::string::npos)
      {
        std::string nbuf;
        // Get the next line and add it to buffer
        GETLINE(code, nbuf);
        lib::pad(nbuf);
        buffer.append(nbuf);

        // Check if a closing parenthesis is given
        pos = nbuf.find(')');
      }

      // Get the function name, with the forward slash
      pos = buffer.find('(');
      std::string fname = buffer.substr(1, pos - 2);
      lib::pad(buffer);

      // Check if the function name is valid, otherwise, syntax error!
      if(!CheckFunctionName(fname))
      {
        fprintf(stderr, "Syntax Error: %s:L%ld:L%ld: Illegal characters in function name.\n", program_file, curpos, nextpos);
        return ERR_PARSE_FAILED;
      }

      // Get the parameter list
      std::vector<std::string, std::string> params;
      if(buffer.back() != ')')
      {
        fprintf(stderr, "Syntax Error: %s:L%ld:%ld: Illegal characters at the end of line; no statements may exist in the same line as the arguments!\n", program_file, curpos, nextpos);
      }
      // Parameter list: buffer[pos + 1 -> end - 1]
      std::string list = buffer.substr(pos + 1, buffer.length() - 2);
    }
    else if(buffer.front() == '@') // Import, Define or some other language structure
    {
    }
    else // Anything else, most likely a statement or some other language structure
    {
    }

    // This is now unnecessary!
#undef GETLINE
    curpos = nextpos;
  }

  return SUCCESS;
}

// Checks for typical syntax errors; currently doesn't do anything
int CircuitLangCTX::CheckCommonSyntaxErrors()
{
  return SUCCESS;
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