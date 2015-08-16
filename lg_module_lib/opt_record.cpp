#include <stdio.h>
#include "opt_record.h"
#include "checker.h"
#include "mem_func.h"
#include "var_def.h"


static int index=0;
static int inty=0;
opt_record_head* new_opt_record_head()
{
	return (opt_record_head*)glist_newGListHead();
}

void free_opt_record_head(opt_record_head **head)
{
    glist_freeGListHead((glist_head_t**)head);
}

void add_opt_record(opt_record_head *head, opt_record *r)
{

	// r->index	= index++;
	// r->inty 	= inty++;

	glist_addNode((glist_head_t*)head, r);

}

// struct opt_record
// {
// 	long int nx, ny, ntha, nphi;
// 	double x0, y0, z0, xrng, yrng;
// 	double *inty;										// intensity profile
// };

void dump_opt_record(struct opt_record *opr)
{
	int i;
	// int inty_count;

	RETURN_ON_NULL(opr);

	// inty_count = opr->nx*opr->ny*opr->ntha*opr->nphi;
	printf("nx: %ld: ny, %ld: ntha, %ld: nphi: %ld, x0: %.2f, y0: %.2f, z0: %.2f, xrng: %.2f, yrng: %.2f, index: %d, inty: %.2f\n",
			opr->nx, 
			opr->ny,
			opr->ntha,
			opr->nphi,
			opr->x0,
			opr->y0,
			opr->z0,
			opr->xrng,
			opr->yrng,
			opr->r_index,
			opr->r_inty
	);
	
	// for(i=0; i<3 && i < inty_count; i++)
	// {
	// 	printf("%.5f, ", opr->inty[i]);
	// }

	// for(i=inty_count-3; i>3 && i < inty_count; i++)
	// {
	// 	printf("%.5f, ", opr->inty[i]);
	// }
}

void dump_opt_record_head(opt_record_head *head)
{
	glist_t *cur;

	cur = ((glist_head_t*)head)->child;

	while(cur != NULL)
	{
		dump_opt_record((opt_record*)cur->vptr);
		cur = cur->next;
	}
}

bool merge_opt_record_head(opt_record_head *dst_head, opt_record_head *src_head)
{

	glist_t *cur;
	RETURNV_ON_NULL(dst_head, false);
	RETURNV_ON_NULL(src_head, false);

	cur = ((glist_head_t*)src_head)->child;

	while(cur != NULL)
	{
		add_opt_record(dst_head, (opt_record *)cur->vptr);
		cur = cur->next;
	}

	return true;
}

struct opt_record* new_opt_record()
{
	struct opt_record* opr;
	opr = new opt_record;
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, opr);

	return opr;
}

void free_opt_record(struct opt_record** opr)
{
	// delete (*opr)->inty;
	delete *opr;
	*opr = NULL;
}
