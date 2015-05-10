#ifndef _DBG_LOG_H_
#define _DBG_LOG_H_

#include "var_type.h"
// _ASSERTE( _CrtCheckMemory( ) ); 
#define CRTDBG_MAP_ALLOC

#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif

#define pl()	do{printf("%s:%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);}while(0)

#define pI(v) 	do{printf("%s: %d\n", #v, v);}while(0)
#define pU(v) 	do{printf("%s: %u\n", #v, v);}while(0)
#define pP(v) 	do{printf("%s: %p\n", #v, v);}while(0)
#define pX(v) 	do{printf("%s: 0x%X\n", #v, v);}while(0)
#define pD(v) 	do{printf("%s: %f\n", #v, v);}while(0)
#define pLU(v) 	do{printf("%s: %lu\n", #v, v);}while(0)
#define pStr(v) 	do{printf("%s: %s\n", #v, v);}while(0)


void dumpRay1(struct ray_trace1*);

#endif