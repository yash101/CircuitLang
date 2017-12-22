#ifndef _PROTECTION_H
#define _PROTECTION_H

// Use protection. You never know when it will save your life!

#ifndef NULL
#define NULL (0x0)
#endif

#include <stdio.h>

template<typename T = void>
class AutoPointer
{
private:
  T* ptr;
  bool array;

public:
  inline AutoPointer(T* pointer, bool arr) :
    ptr(pointer), array(arr)
  {}
  inline void cancel()
  {
    ptr = NULL;
  }
  inline ~AutoPointer()
  {
    if(ptr)
    {
      if(array) delete[] ptr;
      else delete ptr;
    }
  }
};

class FileCloser
{
private:
  FILE* f;
public:
  inline FileCloser() :
    f(NULL)
  {}

  inline FileCloser(FILE* file) :
    f(file)
  {}

  inline void cancel()
  {
    f = NULL;
  }

  inline ~FileCloser()
  {
    if(f) fclose(f);
  }
};

#endif
