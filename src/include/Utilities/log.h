
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#ifdef LOG_ENABLE
#include <string.h>
#include <stdio.h>
#define LOG2CONSOLE(xxx) printf("%s %s %d\n", xxx, __FILENAME__, __LINE__);
#else 
#define LOG2CONSOLE(...)
#endif