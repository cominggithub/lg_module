#include "data_file_util.h"
#include "checker.h"
#include <stdio.h>
#include <memory.h>
#include "dbg_log.h"
#include "glist.h"
#include <string.h>

static FILE* rayCsvFp = NULL;

bool save_ray_source_file(
	const char* fname, 
	data_file_header *dfh, 
	struct ray_traces* rays
)
{
	unsigned int i;
	FILE *fp;
	ray_traces_data data;

	size_t dataSize;
	RETURNV_ON_NULL(dfh, false);
	RETURNV_ON_ZERO(dfh->count, false);
	RETURNV_ON_NULL(rays, false);

	if (dfh->count != rays->nray)
		return false;

	fp = fopen(fname, "wb");
	if (!fp)
		return false;
	
	if (!fwrite(dfh, sizeof(data_file_header), 1, fp))
	{
		return false;
	}

	for(i=0; i<dfh->count; i++)
	{
		data.xr 	= rays->xr[i];
		data.yr 	= rays->yr[i];
		data.zr 	= rays->zr[i];
		data.thar 	= rays->thar[i];
		data.phir 	= rays->phir[i];
		if (!fwrite(&data, sizeof(ray_traces_data), 1, fp))
		{
			return false;
		}
	}

	fflush(fp);
	fclose(fp);

	return true;
}

bool load_ray_source_file_header(
	const char* fname, 
	data_file_header *dfh
)
{
	FILE *fp;
	unsigned int r;
	fp = fopen(fname, "rb");
	if (!fp)
	{
		return false;
	}
	
	if (feof(fp))
	{
		return false;
	}

	if(!fread(dfh, sizeof(data_file_header), 1, fp))
	{
		return false;
	}
	
	fclose(fp);
	return true;
}

bool load_ray_source_file(
	const char* fname, 
	struct ray_traces* rays
)
{
	unsigned int i;
	FILE *fp;
	ray_traces_data data;
	data_file_header dfh;
	fp = fopen(fname, "rb");
	if (!fp)
		return false;
	
	if(!fread(&dfh, sizeof(data_file_header), 1, fp))
		return false;

	rays->nray = 0;
	for(i=0; i<dfh.count; i++)
	{
		if (!fread(&data, sizeof(ray_traces_data), 1, fp))
		{
			return false;
		}
		
		rays->xr[i] 	= data.xr;
		rays->yr[i] 	= data.yr;
		rays->zr[i] 	= data.zr;
		rays->thar[i] 	= data.thar;
		rays->phir[i] 	= data.phir;
	}

	rays->nray = dfh.count;
	fclose(fp);

	return true;
}

void test_ray_source_file()
{
	unsigned int i;
	unsigned int count;
	struct ray_traces rays;
	data_file_header header;
	char fileName[256] = "ray_source.dat.txt";
	
	count 			= 100;
	header.count 	= count;
	header.offset 	= 555;
	strcpy(header.prefix, "p_1-1");
	
	rays.nray 		= count;
	rays.xr 		= new double[rays.nray];
	rays.yr 		= new double[rays.nray];
	rays.zr 		= new double[rays.nray];
	rays.thar 		= new double[rays.nray];
	rays.phir 		= new double[rays.nray];

	for(i=0; i<rays.nray; i++)
	{
		rays.xr[i] 		= i*1;
		rays.yr[i] 		= i*2;
		rays.zr[i] 		= i*3;
		rays.thar[i] 	= i*4;
		rays.phir[i] 	= i*5;
	}

	printf("header prefix: %s\n", header.prefix);
	printf("header count: %ld\n", header.count);
	printf("header offset: %u\n", header.offset);

	save_ray_source_file(fileName, &header, &rays);
	memset(&header, 0, sizeof(data_file_header));

	printf("header prefix: %s\n", header.prefix);
	printf("header count: %ld\n", header.count);
	printf("header offset: %u\n", header.offset);

	load_ray_source_file_header(fileName, &header);

	printf("header prefix: %s\n", header.prefix);
	printf("header count: %ld\n", header.count);
	printf("header offset: %u\n", header.offset);

	for(i=0; i<rays.nray; i++)
	{
		rays.xr[i] 		= 0;
		rays.yr[i] 		= 0;
		rays.zr[i] 		= 0;
		rays.thar[i] 	= 0;
		rays.phir[i] 	= 0;
	}

	load_ray_source_file(fileName, &rays);
	dump_ray_traces(&rays);

}

void dump_ray_traces(struct ray_traces *rays)
{
	unsigned int i;
	for(i=0; i<rays->nray; i++)
	{
		printf("%8u: xr: %.2f, yr: %.2f, zr: %.2f, thar: %.2f, phir: %8.2f\n",
			i,
			rays->xr[i],
			rays->yr[i],
			rays->zr[i],
			rays->thar[i],
			rays->phir[i]
		);
	}
}


// 

bool save_opt_record_file(
	const char* fname, 
	data_file_header *dfh, 
	opt_record_head *head
)
{
	unsigned int i, j;
	int entry_size;
	int opr_size;
	int inty_size;
	FILE *fp;
	glist_t *cur;

	opt_record_data *data;
	struct opt_record *opr;

	RETURNV_ON_NULL(dfh, false);
	RETURNV_ON_NULL(head, false);
    

	size_t dataSize;
	
	fp = fopen(fname, "wb");
	if (!fp)
		return false;
	
	if (!fwrite(dfh, sizeof(data_file_header), 1, fp))
	{
		return false;
	}

	// dont count the size of double point inty


	opr_size 	= sizeof(opt_record) - sizeof(double*);
	entry_size 	= dfh->entry_size;

	inty_size	= dfh->entry_size - opr_size;
	cur 		= ((glist_head_t*)head)->child;
	data 		= (opt_record_data*)malloc(entry_size);

	for(i=0; i<dfh->count && cur != NULL; i++)
	{
		opr = (opt_record*)cur->vptr;
		RETURNV_ON_NULL(opr, false);
		copy_opr_to_opr_data(data, opr, inty_size);

		if (!fwrite(data, entry_size, 1, fp))
		{
			return false;
		}
		cur = cur->next;
	}

	if (i != dfh->count)
	{
		printf("[Error] count not matched: expected : %ld, actual: %u\n", dfh->count, i);
	}

	fflush(fp);
	fclose(fp);

	// free(data);

	return true;

}


bool load_opt_record_file(
	const char* fname, 
	data_file_header *dfh, 
	opt_record_head *head
)
{
	unsigned int i, j;
	int entry_size;
	int opr_size;
	int inty_size;
	glist_t *cur;
	opt_record_data *data;
	struct opt_record *opr;
	FILE *fp;
	size_t read_count;
	double *dd;

	
	fp = fopen(fname, "rb");
	if (!fp)
	{
		return false;
	}
	
	if(!fread(dfh, sizeof(data_file_header), 1, fp))
	{
		return false;
	}

	opr_size 	= sizeof(opt_record) - sizeof(double*);
	entry_size 	= dfh->entry_size;
	inty_size 	= dfh->entry_size - opr_size;
	data 		= (opt_record_data*)malloc(entry_size);

	for(i=0; i<dfh->count; i++)
	{
		opr = new_opt_record();
		
		if (!fread(data, entry_size, 1, fp))
		{
			return false;
		}

		copy_opr_data_to_opr(opr, data, inty_size);
		add_opt_record(head, opr);

	}

	free(data);
	fclose(fp);
	return true;

}

bool merge_opt_record_files(const char *prefix, const char* dst_file, int file_cnt, const char** src_files)
{
	int i;
	int count;
	
	opt_record_head *dst_head;
	opt_record_head *src_head;
	data_file_header dfh;
	glist_t *cur;

	dst_head = new_opt_record_head();
	src_head = new_opt_record_head();

	count = 0;

	for(i=0; i<file_cnt; i++)
	{
		if (!load_opt_record_file(src_files[i], &dfh, dst_head))
		{

			fprintf(stderr, "load opt record file failed: %s\n", src_files[i]);
			return false;
		}
		dump_opt_record_head(dst_head);
		count += dfh.count;
	}

	
// typedef struct _data_file_header
// {
// 	char prefix[256];
// 	unsigned int offset;
// 	int entry_size;
// 	long int count;
// }data_file_header;

	strcpy(dfh.prefix, prefix);
	dfh.offset 		= 0;
	dfh.entry_size 	= sizeof(opt_record);
	dfh.count 		= count;
	save_opt_record_file(dst_file, &dfh, dst_head);
	
	free_opt_record_head(&dst_head);
	
	return true;

}



		
// 		struct opt_record
// {
// 	long int nx, ny, ntha, nphi;
// 	double x0, y0, z0, xrng, yrng;
// 	double *inty;										// intensity profile
// };

void copy_opr_to_opr_data(opt_record_data *data, opt_record* opr, int array_size)
{
	// data->nx 	= opr->nx;
	// data->ny 	= opr->ny;
	// data->ntha 	= opr->ntha;
	// data->nphi 	= opr->nphi;
	// data->x0 	= opr->x0;
	// data->y0 	= opr->y0;
	// data->z0 	= opr->z0;
	// data->xrng 	= opr->xrng;
	// data->yrng	= opr->yrng;

	memcpy(data, opr, sizeof(opt_record_data));
	// memcpy(data->inty, opr->inty, array_size);
}

void copy_opr_data_to_opr(opt_record *opr, opt_record_data* data, int array_size)
{	
	memcpy(opr, data, sizeof(opt_record_data));
	// memcpy(opr->inty, data->inty, array_size);
}

void dump_opt_record_data(opt_record_data *opr)
{
	int i;
	int inty_count;

	RETURN_ON_NULL(opr);

	inty_count = opr->nx*opr->ny*opr->ntha*opr->nphi;
	printf("nx: %ld: ny, %ld: ntha, %ld: nphi: %ld, x0: %.2f, y0: %.2f, z0: %.2f, xrng: %.2f, yrng: %.2f, index:%d, inty:%.2f\n",
			opr->nx, 
			opr->ny,
			opr->ntha,
			opr->nphi,
			opr->x0,
			opr->y0,
			opr->z0,
			opr->xrng,
			opr->yrng,
			opr->index,
			opr->inty

	);
	
	// for(i=0; i<3 && i < inty_count; i++)
	// {
	// 	printf("%.5f, ", opr->inty[i]);
	// }

	// for(i=inty_count-3; i>3 && i < inty_count; i++)
	// {
	// 	printf("%.5f, ", opr->inty[i]);
	// }

	// printf("\n");
}

void open_ray_csv(const char* fname)
{
	if (rayCsvFp == NULL)
	{
		rayCsvFp = fopen(fname, "w");
		if (rayCsvFp != NULL)
		{
			fprintf(rayCsvFp, 
				"num,"
				"ngaus, inty, n1, n2, "
				"xr, yr, zr, "
				"thar, phir, "
				"nx, ny, nz, "
				"opt_index, opt_inty, "
				"type"
				"\n"
			);
		}
	}
}

void close_ray_csv()
{
	if (rayCsvFp != NULL)
	{
		fclose(rayCsvFp);
	}

	rayCsvFp = NULL;
}

void append_ray_to_csv(const char *prefix, ray_trace1 *ray)
{
	RETURN_ON_NULL(rayCsvFp);

	fprintf(rayCsvFp, 
		"[%s], %ld, %f, %f, %f, "
		"%f, %f, %f, "
		"%f, %f, "
		"%f, %f, %f\n",
		prefix,
		ray->ngaus, ray->inty, ray->n1, ray->n2,
		ray->xr, ray->yr, ray->zr,
		ray->thar, ray->phir,
		ray->nx, ray->ny, ray->nz
	);

	fflush(rayCsvFp);
}

void append_ray_and_opt_record_to_csv(const char *prefix, ray_trace1 *ray, opt_record *opr)
{
	RETURN_ON_NULL(rayCsvFp);

	if (opr != NULL)
	{
		fprintf(rayCsvFp, 
			"[%s], %ld, %f, %f, %f, "
			"%f, %f, %f, "
			"%f, %f, "
			"%f, %f, %f, "
			"%d, %f"
			"\n",
			prefix,
			ray->ngaus, ray->inty, ray->n1, ray->n2,
			ray->xr, ray->yr, ray->zr,
			ray->thar, ray->phir,
			ray->nx, ray->ny, ray->nz,
			opr->r_index, opr->r_inty
		);
	}
	else
	{
		fprintf(rayCsvFp, 
			"[%s], %ld, %f, %f, %f, "
			"%f, %f, %f, "
			"%f, %f, "
			"%f, %f, %f\n",
			prefix,
			ray->ngaus, ray->inty, ray->n1, ray->n2,
			ray->xr, ray->yr, ray->zr,
			ray->thar, ray->phir,
			ray->nx, ray->ny, ray->nz
	);
	}

	fflush(rayCsvFp);
}

void append_ray_and_opt_record_to_csv_type(const char *prefix, ray_trace1 *ray, opt_record *opr, int type)
{
	RETURN_ON_NULL(rayCsvFp);

	if (opr != NULL)
	{
		fprintf(rayCsvFp, 
			"[%s], %ld, %f, %f, %f, "
			"%f, %f, %f, "
			"%f, %f, "
			"%f, %f, %f, "
			"%d, %f, %d"
			"\n",
			prefix,
			ray->ngaus, ray->inty, ray->n1, ray->n2,
			ray->xr, ray->yr, ray->zr,
			ray->thar, ray->phir,
			ray->nx, ray->ny, ray->nz,
			opr->r_index, opr->r_inty, type
			
		);
	}
	else
	{
		fprintf(rayCsvFp, 
			"[%s], %ld, %f, %f, %f, "
			"%f, %f, %f, "
			"%f, %f, "
			"%f, %f, %f, %d\n",
			prefix,
			ray->ngaus, ray->inty, ray->n1, ray->n2,
			ray->xr, ray->yr, ray->zr,
			ray->thar, ray->phir,
			ray->nx, ray->ny, ray->nz, type
	);
	}

	fflush(rayCsvFp);
}

bool load_matrix(const char *filename, int nx, int ny, double *data)
{
	int i;
  	int j;

	/*matrix*/
	/*Use double , you have floating numbers not int*/

	FILE *file;
	
	
	file=fopen(filename, "r");
  	if (!file)
  	{
  		fprintf(stderr, "cannot open %s\n", filename);
  		return false;
  	}
  	
 	for(i = 0; i < ny; i++)
  	{
		for(j = 0; j < nx; j++) 
		{
			if (!fscanf(file, "%lf ", &data[i*ny+j])) 
  			{
  				fprintf(stderr, "read matrix: %s, fail\n", filename);
				break;
  			}
      	
       		// printf("%f ", data[i*ny+j]);
       		
      	}
      	// printf("\n");

  	}
  	fclose(file);

  	return true;
}


// struct dot_position
// {
// 	long int ndot;
// 	long int partnx, partny;									// partition all dots into grids=npartnx*npartny
// 	long int *partaccni, *partindx;								// partaccni is for accumlate dots for each grid, and partindx is the responsible array index of xd/yd;
// 	double *xd, *yd;											// positions of dots
// };

// 	// allocate memory for global dot position
// 	dpos->ndot = n_dots;
// 	dpos->partnx = partn_x;
// 	dpos->partny = partn_y;
// 	dpos->xd = new double[dpos->ndot];
// 	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
// 	dpos->yd = new double[dpos->ndot];
// 	if( dpos->xd == nullptr ) { printf("allocmem_dot_position: dot potision error\n"); exit(0); }
// 	dpos->partaccni = new long int[dpos->partnx*dpos->partny];
// 	if( dpos->partaccni == nullptr ) { printf("allocmem_dot_position: accumulate-dot-number matrix error\n"); exit(0); }
// 	dpos->partindx = new long int[dpos->ndot];
// 	if( dpos->partindx == nullptr ) { printf("allocmem_dot_position: dot-index matrix error\n"); exit(0); }
// 	return;


void save_dot_position_file(dot_position *dpos)
{
	FILE *fp;
	int i,j;
	fp = fopen("dot_position.txt", "w");
	if (fp == NULL)
	{
		return;
	}


	fprintf(fp, "ndot: %ld\n", dpos->ndot);
	fprintf(fp, "partnx: %ld\n", dpos->partnx);
	fprintf(fp, "partny: %ld\n", dpos->partny);

	fprintf(fp, "\n\nxd:\n");
	for(i=0; i<dpos->ndot; i++)
	{
		fprintf(fp, "%f ", dpos->xd[i]);
	}

	fprintf(fp, "\n\nyd:\n");
	for(i=0; i<dpos->ndot; i++)
	{
		fprintf(fp, "%f ", dpos->yd[i]);
	}

	fprintf(fp, "\n\npartaccni:\n");
	for(i=0; i<dpos->partny; i++)
	{
		for(j=0; j<dpos->partnx; j++)
		{
			fprintf(fp, "%ld ", dpos->partaccni[i*dpos->partnx+j]);
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\npartindx:\n");
	for(i=0; i<dpos->ndot; i++)
	{
		fprintf(fp, "%ld ", dpos->partindx[i]);
	}

	fflush(fp);
	fclose(fp);
}


void save_opt_record_txt_file(
	const char *fname,
	opt_record *opr
)
{
	int i, j, k, l;
	FILE *fp;

	if (opr == NULL)
	{
		return;
	}

	fp = fopen(fname, "w");
	if (fp == NULL)
	{
		return;
	}

	fprintf(fp, "nx: %ld, ny = %ld, ntha = %ld, nphi = %ld\n", opr->nx, opr->ny, opr->ntha, opr->nphi);
	for (i=0; i<opr->nx; i++)
	{
		for (j=0; j<opr->ny; j++)
		{
			fprintf(fp, "\nnx = %d, ny = %d\n", i, j);
			for (k=0; k<opr->ntha; k++)
			{
				for(l=0; l<opr->nphi; l++)
				{
					long index = i*opr->ny + j*opr->ntha + k*opr->nphi + l;
					fprintf(fp, "%f ", opr->inty[index]);
				}
				fprintf(fp, "\n");
			}
		}
		
	}
	fflush(fp);
	fclose(fp);
	
}


bool save_opt_record_dat_file(
	const char *fname,
	opt_record *opr
)
{
	FILE *fp;
	size_t inty_size = 0;

	// fp = fopen(fname, "wb");
	

	RETURNV_ON_NULL(fname, false);
	RETURNV_ON_NULL(opr, false);

	fp = fopen(fname, "wb");
	
	if (fp == NULL)
	{
		return false;
	}

	
	inty_size = sizeof(double)*opr->nx*opr->ny*opr->ntha*opr->nphi;

	if (!fwrite(opr, sizeof(opt_record) - sizeof(double*), 1, fp))
	{
		fflush(fp);
		fclose(fp);
		return false;
	}

	if (!fwrite(opr->inty, inty_size, 1, fp))
	{
		fflush(fp);
		fclose(fp);
		return false;
	}

	dump_opt_record(opr);

	fflush(fp);
	fclose(fp);
	return true;
}

bool load_opt_record_dat_file(
	const char *fname,
	opt_record *opr
)
{
	FILE *fp;
	size_t inty_size = 0;

	
	
	RETURNV_ON_NULL(fname, false);
	RETURNV_ON_NULL(opr, false);
	
	
	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		fclose(fp);
		return false;	
	}


	if(!fread(opr, sizeof(opt_record) - sizeof(double*), 1, fp))
	{
		fclose(fp);
		return false;
	}

	inty_size = sizeof(double)*opr->nx*opr->ny*opr->ntha*opr->nphi;
	opr->inty = new double[opr->nx*opr->ny*opr->ntha*opr->nphi];
	if( opr->inty == nullptr ) 
	{ 
		printf("allocmem_record: light recording error\n");
		fclose(fp);
		return false;
	}

	if (!fread(opr->inty, inty_size, 1, fp))
	{
		fclose(fp);
		return false;
	}

	dump_opt_record(opr);

	fclose(fp);
	return true;

}
