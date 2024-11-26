#ifndef __common_h__
#define __common_h__


#include <assert.h>
#include <stdlib.h>


#define Malloc(s)({void *p = malloc(s); assert(p != NULL; p;)})
#define Time-GetSeconds() ({struct timeval t; int rc = getimeofday(&t, NULL); assert(rc == 0); (double) t.tv_sec + (double) t.tv_sec/1e6;})



#endif 
