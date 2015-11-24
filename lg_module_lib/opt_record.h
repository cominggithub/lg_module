#ifndef _OPT_RECODER_H_
#define _OPT_RECODER_H_

#include "var_type.h"
#include "glist.h"


// opr->nx = nx_rcd;
// 	opr->ny = ny_rcd;
// 	opr->ntha = ntha_rcd;
// 	opr->nphi = nphi_rcd;
// 	opr->x0 = xrcd_or;
// 	opr->y0 = yrcd_or;
// 	opr->z0 = zrcd_or;
// 	opr->xrng = xrcd_rng;
// 	opr->yrng = yrcd_rng;


// opr->nx *
// opr->ny *
// opr->ntha *
// opr->nphi

// #define ACTUAL_OPT_RECORD_SIZE (sizeof(struct opt_record)-sizeof(double*)+sizeof(double)*nx_rcd*ny_rcd*ntha_rcd*nphi_rcd)
#define ACTUAL_OPT_RECORD_SIZE (sizeof(struct opt_record))

#define OPT_RECORD_INTY_SIZE(o) (o->nx*o->ny*o->ntha*o->nphi)

typedef glist_t opt_record_head;



opt_record_head* new_opt_record_head();
void add_opt_record(opt_record_head *head, opt_record *r);
bool merge_opt_record_head(opt_record_head *dst_head, opt_record_head *src_head);
bool merge_opt_record(opt_record *dst, opt_record *src);
void free_opt_record_head(opt_record_head **head);
struct opt_record* new_opt_record();
void free_opt_record(struct opt_record**);
void dump_opt_record_head(opt_record_head *head);
void dump_opt_record(struct opt_record *opr);

#endif