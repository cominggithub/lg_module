#ifndef _CHECKER_H_
#define _CHECKER_H_

#include "dbg_log.h"

#define RETURN_ON_NULL(v) do{if (v==NULL){ pl(); fprintf(stderr, "%s is null", #v); return ;}}while(0)
#define RETURN_ON_ZERO(v) do{if (v==0){ pl(); fprintf(stderr, "%s is zero", #v); return ;}}while(0)

#define RETURNR_ON_NULL(v, r) do{if (v==NULL){ pl(); fprintf(stderr, "%s is null", #v); return r;}}while(0)
#define RETURNR_ON_ZERO(v, r) do{if (v==0){ pl(); fprintf(stderr, "%s is zero", #v); return r;}}while(0)
#endif