#ifndef _CONFIG_H
#define _CONFIG_H

// Should be defined through a compile-time flag
#ifndef OS_FILEPATH_SEPARATOR
#ifdef _WIN32
#define OS_FILEPATH_SEPARATOR '\\'
#else
#define OS_FILEPATH_SEPARATOR '/'
#endif
#endif

#endif