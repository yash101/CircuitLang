#ifndef _PROTECTION_H
#define _PROTECTION_H

// Use protection. You never know when it will save your life!

#ifndef NULL
#define NULL (0x0)
#endif

#include <stdio.h>

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
