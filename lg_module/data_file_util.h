#ifndef _LG_MULTI_PROCESSOR_H_
#define _LG_MULTI_PROCESSOR_H_

#include "var_type.h"
typedef struct _data_file_header
{
	char prefix[256];
	unsigned int offset;
	long int count;
}data_file_header;

bool save_ray_source_file(
	const char* fname, 
	data_file_header *dfh, 
	struct ray_traces* rays
);

bool load_ray_source_file_header(
	const char* fname, 
	data_file_header *dfh
);

bool load_ray_source_file(
	const char* fname, 
	struct ray_traces* rays
);


void test_ray_source_file();
void dump_ray_traces(struct ray_traces *rays);
#endif