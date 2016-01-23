#include "data_file_util.h"

#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include "malloc.h"
#endif
#include "time.h"
// #include "tchar.h"

// for private header
#include "conf_util.h"
#include "var_def.h"			// define parameters and structe variables
#include "plt_figure.h"			// call gnuplot for ploting figure
#include "ini_var.h"			// initialize variables
#include "mem_func.h"			// allocate/deallocate memory
#include "microstr.h"			// local microstructure profile
#include "gen_source_ray.h"		// generate light-source rays
#include "globalstr.h"
#include "den_to_pos.h"			// module for transferring dot_density to dot_position
#include "debug_fun.h"			// just for debugging individual module.
#include "Module_IV.h"
#include "dbg_log.h"
#include "time_util.h"
#include "data_file_util.h"
#include "checker.h"

int main(int argc, const char** argv)
{
	int i;
	int count;
	char prefix[256];

	char dst_file[256];
	char *opt_record_name[256];
	char tmp_output[256];

	// define variables
	opt_source ops;					// for optic source
	ray_traces rays;				// for samplings of ray tracing
	char fpname[256];					// for reading parameters
	char paramFName[256];
	time_t t;
	struct tm *now;
	opt_record opr_tmp;
	opt_record opr_merged;

	t 		= time(0);
	now 	= localtime(&t);
	count 	= 1;

	if (argc != 3)
		return 1;

	count = atoi(argv[1]);
	strcpy(prefix, argv[2]);

	// printf("    prefix: %s\n", prefix);
	// printf("proc count: %d\n", count);

	srand((unsigned)time(NULL));	// initiate rand seed
	// read in setup-parameters
	sprintf(paramFName, "%s/parameters.txt", prefix);
	if (!read_setup(paramFName, prefix))
		return 1;
	
	setTmpOutputFolder(tmp_output);

	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr_tmp);
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr_merged);

	for(i=0; i<count; i++)
	{
		opt_record_name[i] = (char*)malloc(100);
		sprintf(opt_record_name[i], "%s/process_%d/%s", prefix, i, output_opt_record_dat);
		load_opt_record_dat_file(opt_record_name[i], &opr_tmp);
		merge_opt_record(&opr_merged, &opr_tmp);
	}


	sprintf(dst_file, "%s/%s", prefix, output_opt_record_dat);
	save_opt_record_dat_file(dst_file, &opr_merged);
	pStr(dst_file);

	sprintf(dst_file, "%s/%s", prefix, output_opt_record_txt);
	save_opt_record_txt_file_pos(dst_file, &opr_merged);
	pStr(dst_file);

	deallocmem_record(&opr_tmp);
	deallocmem_record(&opr_merged);

	return 0;
}