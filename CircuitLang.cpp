#include "stdafx.h"
#include "stringproc.h"
#include "runtime.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
/*
  Entry point for CircuitLang
*/

int main()
{
  std::string x = "hello\nworld!\n\nhello\world\n";
  std::cout << x << std::endl << std::endl;

  std::stringstream s(x);
  std::string buffer;
  while(std::getline(s, buffer))
  {
    std::cout << "[" << buffer << "]" << std::endl;
  }
  system("pause");
  return 0;
}