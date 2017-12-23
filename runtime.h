#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>

#include "datatypes.h"

namespace runtime
{
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
    // Function type
    int tp;
    CL_rt_func_t func;
  } CL_rt_func_container_t;

  class CircuitLangCTX
  {
    // Private structures and typedefs/etc, builtin functions
  private:
    static const int func_type_undef = 0;    // default initializer for functions; a regular function type
    static const int func_type_func = 1;     // 
    static const int func_type_jmpif = 2;
    static const int func_type_jmpifnot = 3;

    // Built in primitive functions
    // Arithmetic
    static int CL_builtin_arith_add(void* args); // add
    static int CL_builtin_arith_sub(void* args); // subtract
    static int CL_builtin_arith_mul(void* args); // multiply
    static int CL_builtin_arith_div(void* args); // divide
    static int CL_builtin_arith_shl(void* args); // shift left
    static int CL_builtin_arith_shr(void* args); // shift right
    static int CL_builtin_arith_or(void* args);  // bitwise or
    static int CL_builtin_arith_not(void* args); // bitwise not
    static int CL_builtin_arith_and(void* args); // bitwise and
    static int CL_builtin_arith_xor(void* args); // bitwise xor

    // Comparison
    static int CL_builtin_compare_eq(void* args);  // compare equal (==)
    static int CL_builtin_compare_neq(void* args); // compare not equal (=/=)
    static int CL_builtin_compare_lt(void* args);  // compare less than
    static int CL_builtin_compare_gt(void* args);  // compare greater than
    static int CL_builtin_compare_lte(void* args); // compare less than or equal
    static int CL_builtin_compare_gte(void* args); // compare greater than or equal

    // Basic Ops
    static int CL_builtin_basic_create_variable(void* args); // defines a variable
    static int CL_builtin_basic_set_variable(void* args);    // sets a variable
    static int CL_builtin_basic_define_globvar(void* args);  // defines a global variable
    static int CL_builtin_basic_get_globvar(void* args);     // retrieves a global variable

    // Input functions
    static int CL_builtin_readchar(void* args);   // reads char from stdin
    static int CL_builtin_readint(void* args);    // reads int from stdin
    static int CL_builtin_readlong(void* args);   // reads long from stdin
    static int CL_builtin_readfloat(void* args);  // reads float from stdin
    static int CL_builtin_readdouble(void* args); // reads double from stdin

    // Output functions
    static int CL_builtin_printchar(void* args); // prints a char
    static int CL_builtin_print(void* args);     // prints something
    static int CL_builtin_println(void* args);   // prints something and then prints a newline

    // Functions
    static int CL_builtin_call_function(void* args); // calls a function

    // Toolkit functions
  protected:
    static bool ValidFirstLetterInFunctionName(char ch);
    static bool ValidLetterInFunctionName(char ch);
    static bool CheckFunctionName(std::string fn);

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
    int CheckCommonSyntaxErrors();
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