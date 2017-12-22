#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <map>
#include <string>
#include <vector>

#include "datatypes.h"

namespace runtime
{
  // temporary to allow program compilation
  typedef int func;
  namespace opcodes
  {
    extern std::map<std::string, func>;
  }

  // This function resolves a path to a file being imported
  std::string resolve_file_location(std::string reference, std::string path);

  /*
  Execution context of a program; Contains the following:
    - Program code (processed)
    - Execution pipeline (stack/queue/whatever's fastest)
    - Function table
    - Tickno
    - Data storage
  */
  class CircuitLangCTX
  {
  private:
    std::string program_file;
    std::string program_code;

    unsigned long program_counter;   // Clock, optional for implementation, but can be useful for performance testing

  public:
    // Run program from code in string or file (true = file, false = string)
    CircuitLangCTX(std::string str, bool file);
    CircuitLangCTX();

    int LoadFromFile();
    int LoadFromCodeString();   // Imports don't work when running from String

    int Run();
  };
}

#endif