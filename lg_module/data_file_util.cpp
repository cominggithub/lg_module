#include "data_file_util.h"
#include "checker.h"
#include <stdio.h>
#include <memory.h>
#include "dbg_log.h"


typedef struct _ray_traces_data
{
	double xr, yr, zr, thar, phir;
}ray_traces_data;

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
	RETURNR_ON_NULL(dfh, false);
	RETURNR_ON_ZERO(dfh->count, false);
	RETURNR_ON_NULL(rays, false);

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
	printf("header count: %u\n", header.count);
	printf("header offset: %u\n", header.offset);

	save_ray_source_file(fileName, &header, &rays);
	memset(&header, 0, sizeof(data_file_header));

	printf("header prefix: %s\n", header.prefix);
	printf("header count: %u\n", header.count);
	printf("header offset: %u\n", header.offset);

	load_ray_source_file_header(fileName, &header);

	printf("header prefix: %s\n", header.prefix);
	printf("header count: %u\n", header.count);
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