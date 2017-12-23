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

  std::stringstream buffer(program_code);
  std::string tmp;

  // Break into lines
  std::vector<std::string> lines;
  while(std::getline(buffer, tmp))
  {
    // tmp has a line
    // pad the line
    lib::pad(tmp);
    // add to the line vector
    lines.push_back(tmp);
  }

  for(size_t i = 0; i < lines.size(); i++)
  {
  }

  return SUCCESS;
}

int CircuitLangCTX::CheckCommonSyntaxErrors()
{
  // Check brackets, braces and parentheses and quotation marks to see if they have matches
  std::stack<char> stk;
  size_t num = program_code.size();
  for(size_t i = 0; i < num; i++)
  {

    // Check quotes first. If we are inside a quote, traverse to the end
    if(program_code[i] == '"')
    {
      while(true)
      {
        if(i < num && program_code[i] != '"') // Iterate until finding the end
        {
          i++;
        }
        else
        {
          if(program_code[i - 1] == '\\') // Check to see if false positive (escaped quote)
          {
            i++;
          }
          else
          {
            if(i == num - 1)
            {
              fprintf(stderr, "Error, expected double quote, received EOL/EOF!\n");
              return ERR_PRECHECK_FAILED;
            }
            break;  // We've reached the end of the string
          }
        }
      }
    }

    else if(program_code[i] == '\'')
    {
      if(program_code[i + 2] == '\'')
      {
        i += 2;
        continue;
      }
      else
      {
        // Escape sequence?
        if(program_code[i + 1] == '\\')
        {
          // Hex escape sequence?
          if(program_code[i + 2] == 'x')
          {
            if(program_code[i + 5] == '\'')
            {
              i += 5;
              continue;
            }
            else
            {
              fprintf(stderr, "Error, expected single quote, received EOL/EOF!\n");
              return ERR_PRECHECK_FAILED;
            }
          }
          // Non-hex escape sequence? Note that escape sequences will be replaced
          // by the parser
          else
          {
            // '\a'
            if(program_code[i + 3] == '\'')
            {
              i += 3;
            }
            else
            {
              fprintf(stderr, "Error, expected single quote, received EOL/EOF");
              return ERR_PRECHECK_FAILED;
            }
          }
        }
        else
        {
          fprintf(stderr, "Error, expected single quote!\n");
          return ERR_PRECHECK_FAILED;
        }
      }
    }

    // Check if bracket-type thing
    else if(program_code[i] == '{' ||
            program_code[i] == '[' ||
            program_code[i] == '(')
    {
      stk.push(program_code[i]);
    }

    else if(program_code[i] == '}' ||
            program_code[i] == ']' ||
            program_code[i] == ')')
    {
      char ch = stk.top();
      stk.pop();
      if(ch == '{' && program_code[i] == '}')
      {
      }
      else if(ch == '[' && program_code[i] == ']')
      {
      }
      else if(ch == '(' && program_code[i] == ')')
      {
      }
      else
      {
        fprintf(stderr, "Error, unmatched '%c'!\n", ch);
        return ERR_PARSE_FAILED;
      }
    }
  }
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