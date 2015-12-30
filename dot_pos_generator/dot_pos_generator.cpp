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

bool tetgen_node()
{
	dot_density dden;
	dot_position dpos;
	
	if (!read_setup("parameters.txt", NULL))
	{
		fprintf(stderr, "cannot read parameters files\n");
		return 1;
	}

	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);

	set_dot_density(&dden, 0.001);
	den2pos_tetgen("KK", &dden, &dpos, hex_bl, hex_lng);
	part_dots(&dpos);

	return 0;
}

int main(int argc, char** argv)
{
	char prefix[256];
	char node_prefix[256];
	char node_fname[256];
	char dpos_fname[256];
	dot_density dden;
	dot_position dpos;
	double density;

	if (argc == 1)
	{
		tetgen_node();
		return 0;
	}

	strcpy(node_prefix, argv[1]);
	density = atof(argv[2]);

	printf("prefix: %s, density: %f(%s)\n", node_prefix, density, argv[2]);
	
	if (!read_setup("parameters.txt", NULL))
	{
		fprintf(stderr, "cannot read parameters files\n");
		return 1;
	}

	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);

	// load dot_density
	// load_density("dot_density.txt", &dden);
	set_dot_density(&dden, density);

	// generate smesh and mtr file
	// use tetgen to generate node file
	printf("generate dots\n");
	set_start_time("generate_pos_file");
	// generate_pos_file(&dden, node_prefix);
	set_end_time("generate_pos_file");

	sprintf(node_fname, "%s.1.node", node_prefix);
	sprintf(dpos_fname, "%s.dot_position.dat", node_prefix);
	printf("load_dpos\n");
	set_start_time("load_dpos");
	if (!load_dpos(node_fname, &dpos, hex_bl, hex_lng))
	{
		fprintf(stderr, "fload dpos failed");
	}
	set_end_time("load_dpos");
	
	
	set_start_time("part dots");
	printf("part dots\n");
	part_dots(&dpos);
	set_end_time("part dots");

	set_start_time("save dots");
	printf("save dots\n");
	save_dot_position_dat_file(dpos_fname, &dpos);
	set_end_time("save dots");

	printf("\n");
	print_all_execution_time();
	
	return 0;
}
