#ifndef _LG_MULTI_PROCESSOR_H_
#define _LG_MULTI_PROCESSOR_H_

#include "var_type.h"
#include "opt_record.h"

typedef struct _data_file_header
{
	char prefix[256];
	unsigned int offset;
	int entry_size;
	long int count;
}data_file_header;

typedef struct _ray_traces_data
{
	double xr, yr, zr, thar, phir, inty;
}ray_traces_data;

typedef struct _opt_record_data
{
	long int nx, ny, ntha, nphi;
	double x0, y0, z0, xrng, yrng;
	int index;
	double inty;
	// double inty[0];										// intensity profile

}opt_record_data;


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

bool save_opt_record_file(
	const char* fname, 
	data_file_header *dh, 
	opt_record_head *h
);

bool load_opt_record_file(
	const char* fname, 
	data_file_header *dfh, 
	opt_record_head *head
);

bool merge_opt_record_files(
	const char *prefix,
	const char* dest_file, 
	int file_cnt, 
	const char** src_files
);

void copy_opr_to_opr_data(
	opt_record_data *data, 
	opt_record* opr, 
	int array_size
);

void copy_opr_data_to_opr(
	opt_record *opr, 
	opt_record_data* data, 
	int array_size
);

void save_opt_record_txt_file(const char *fname, opt_record *opr);


bool save_opt_record_dat_file(const char *fname, opt_record *opr);

void save_opt_record_txt_file_pos(const char *fname, opt_record *opr);


bool load_opt_record_dat_file(const char *fname, opt_record *opr);

void save_opt_record_txt_file_ang(const char *fname, opt_record *opr);

bool save_dot_position_dat_file(const char *fname, dot_position *dpos);
bool load_dot_position_dat_file(const char *fname, dot_position *dpos);
bool save_dot_position_txt_file(const char *fname, dot_position *dpos);
bool load_dot_position_txt_file(const char *fname, dot_position *dpos);

void test_ray_source_file();
void dump_ray_traces(struct ray_traces *rays);
void dump_opt_record_data(opt_record_data *opr);

void open_ray_csv(const char* fname);
void close_ray_csv();
void append_ray_to_csv(const char *prefix, ray_trace1 *ray);
void append_ray_and_opt_record_to_csv(const char *prefix, ray_trace1 *ray, opt_record *opr);
void append_ray_and_opt_record_to_csv_type(const char *prefix, ray_trace1 *ray, opt_record *opr, int type);
void append_ray_and_opt_record_to_csv_lstr(const char *prefix, ray_trace1 *ray, local_str *lstr);
bool load_matrix(const char *filename, int nx, int ny, double *data);


void save_dot_position_file(dot_position *dpos);


#endif