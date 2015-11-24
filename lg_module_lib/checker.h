#ifndef _CHECKER_H_
#define _CHECKER_H_

#include "dbg_log.h"

#define RETURN_ON_NULL(v) do{if (v==NULL){ pl(); fprintf(stderr, "%s is null\n", #v); return ;}}while(0)
#define RETURN_ON_ZERO(v) do{if (v==0){ pl(); fprintf(stderr, "%s is zero\n", #v); return ;}}while(0)

#define RETURNV_ON_NULL(v, r) do{if (v==NULL){ pl(); fprintf(stderr, "%s is null\n", #v); return r;}}while(0)
#define RETURNV_ON_ZERO(v, r) do{if (v==0){ pl(); fprintf(stderr, "%s is zero\n", #v); return r;}}while(0)

#define RETURNV_ON_NEQ(v1, v2, r) do{if (v1!=v2){ pl(); fprintf(stderr, "%s and %s are not equals\n", #v1, #v2); return r;}}while(0)
#endif