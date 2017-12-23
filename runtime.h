#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>

#include "datatypes.h"

namespace runtime
{
  // These are built-in functions in the language!
  namespace builtins
  {
    // Input functions
    int CL_builtin_readchar(void* args);
    int CL_builtin_readint(void* args);
    int CL_builtin_readlong(void* args);
    int CL_builtin_readfloat(void* args);
    int CL_builtin_readdouble(void* args);

    // Output functions
    int CL_builtin_printchar(void* args);
    int CL_builtin_print(void* args);
    int CL_builtin_println(void* args);
  }

  /*
  Execution context of a program; Contains the following:
    - Program code (processed)
    - Execution pipeline (stack/queue/whatever's fastest)
    - Function table (hashmap of function bytecode)
    - Clock (program counter / tick counter)
    - Data storage (vault)
  */

  // Every function in CircuitLang code is converted to a list of C/C++ functions, which are stored as a list of functions
  typedef int(*CL_rt_func_t)( void* args );

  // This holds a function, as well as information about it
  typedef struct
  {
    int tp;
    size_t jmp_pos;
    CL_rt_func_t func;
  } CL_rt_func_container_t;

  class CircuitLangCTX
  {
    // Private structures and typedefs/etc.
  private:
    static const int func_type_undef = 0;
    static const int func_type_func = 1;
    static const int func_type_jmpif = 2;
    static const int func_type_jmpifnot = 3;

  private:
    // Holds the program code and the program file
    std::string program_file;
    std::string program_code;

    // When a program file is parsed, each operation is translated into an "instruction"
    // An "instruction" is a structure with a C/C++ function, which performs work, a list
    // of parameters, and some information for the runtime executor.
    // In fact, if wanted, these instructions can be directly (or indirectly) converted to
    // machine code and executed directly, allowing a faster-than-C performance.
    std::vector<CL_rt_func_container_t> instruction_vector;

    // This resolves function jump points
    // A function point is the "address" of the first instruction of a function,
    // stored in the instruction_vector
    std::unordered_map<std::string, size_t> jmp_resolver;

    // The clock, which ticks upon the completion of a level of execution
    // Remember that functions are executed "simultaneously"
    unsigned long program_counter;

    // This function parses a program and populates instruction_vector and jmp_resolver
    int ParseProgram();

  public:
    // Run program from code in string or file (true = file, false = string)
    CircuitLangCTX(std::string str, bool file);
    CircuitLangCTX();

    int LoadFromFile(std::string file);
    int LoadFromCodeString(std::string code);   // Imports don't work when running from String

    int Run();
  };

  // This function resolves a path to a file being imported
  std::string resolve_file_location(std::string reference, std::string path);
}

#endif