#include <stdio.h>
#include <string.h>
#include "den_to_pos.h"
#include "var_def.h"
#include "var_type.h"
#include "mem_func.h"
#include "ini_var.h"
#include "data_file_util.h"
#include "globalstr.h"
#include "time_util.h"
#include "dot_pos.h"

bool tetgen_dot_pos(const char* file_prefix, bool sorted, double density)
{
	dot_density dden;
	dot_position dpos;
	char dot_pos_data_fname[256];
	
	
	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);

	set_dot_density(&dden, density);
	if (sorted)
	{
		den2pos_tetgen_sorted(file_prefix, &dden, &dpos, hex_bl, hex_lng);
		set_start_time("part_dots");
		part_dots(&dpos);
		set_end_time("part_dots");
	}
	else
	{
		den2pos_tetgen(file_prefix, &dden, &dpos, hex_bl, hex_lng);	
		set_start_time("part_dots");
		part_dots_unsorted(&dpos);
		set_end_time("part_dots");
	}
	

	sprintf(dot_pos_data_fname, "%s.dot_pos.dat", file_prefix);
	save_dot_position_dat_file(dot_pos_data_fname, &dpos);
	// save_dot_position_txt_file("o.dot_position.txt", &dpos);
	return 0;
}



// 1 parameters 
// 2 prefix
// 3 density
// 4 unsorted
// 
void print_usage()
{
	printf("dot_pos_generator <parameters file> <file_preifx> <density> [-t]\n");
}

int main(int argc, char** argv)
{
	char node_fname[256];
	char dpos_fname[256];
	char para_fname[256];
	char file_prefix[256];
	double density;

	
	set_start_time("Total");
	if (argc < 4)
	{
		print_usage();
		system("pause");
		return 0;
	}

	strcpy(para_fname, argv[1]);
	strcpy(file_prefix, argv[2]);
	density = atof(argv[3]);

	if (!read_setup(para_fname, NULL))
	{
		fprintf(stderr, "cannot read parameters files: %s\n", para_fname);
		return 1;
	}

	if (argc == 5 && strcmp("-t", argv[4]) == 0)
	{
		tetgen_dot_pos(file_prefix, false, density);
	}
	else
	{
		tetgen_dot_pos(file_prefix, true, density);
	}

	set_end_time("Total");
	
	print_all_execution_time();
	system("pause");
	return 0;
}
