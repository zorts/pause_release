// -*- C++ -*-

#include "mvsalloc.h"
#include <stdlib.h>

#ifdef zOS
void* getmain31(int size, int subpool)
{
  void* data;
  __asm(" SYSSTATE PUSH \n"
        " SYSSTATE AMODE64=YES \n"
        " BASR 13,0 \n"
        " USING *,13 \n"
        " STORAGE OBTAIN,LENGTH=(%1),SP=(%2),LOC=31,CALLRKY=YES\n" 
        " DROP 13 \n"
        " SYSSTATE POP \n"
	" LGR %0,1"
        : "=r"(data)
        : "r"(size), "r"(subpool)
        : "r0 r1 r13 r14 r15"
        );
  return data;
}

int freemain31(void * data, int size, int subpool)
{
  int rc = 0;
  __asm(" SYSSTATE PUSH \n"
        " SYSSTATE AMODE64=YES \n"
        " BASR 13,0 \n"
        " USING *,13 \n"
        " STORAGE RELEASE,LENGTH=(%1),SP=(%2),ADDR=(%3),CALLRKY=YES\n"
        " DROP 13 \n"
        " SYSSTATE POP \n"
        " LGR %0,15 \n"
	: "=r"(rc)
	: "r"(size), "r"(subpool), "r"(data)
        : "r0 r1 r13 r14 r15"
        );
  return rc;
}

#else
void* getmain31(int size, int subpool)
{
  return malloc((size_t) size);
}

int freemain31(void *data, int size, int subpool)
{
  free(data);
  return 0;
}
#endif  
