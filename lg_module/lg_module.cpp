
// for regular C header
//#include "stdio.h"
#include <string.h>
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
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
#include "ray_tracer.h"
#include "opt_record.h"
#include "data_file_util.h"
#include "box_check.h"
// // function for memory arrangement
// void read_setup(char *fpname);

// // function for moudles
// void gen_source_ray(opt_source *ops, ray_traces *rays);
// void read_microstr(char *fname, local_str *lstr);
// void find_str_hit_global(ray_trace1 *ray1, dot_position *dpos, opt_record *opr);				// find the intersection of ray and global geometry
// void find_str_hit_local(ray_trace1 *ray1, local_str *lstr);										// find the intersection of ray and local microstructure
// void part_dots(dot_position *dpos);																// partition random dots into regular grids
// void den2pos(dot_density *dden, dot_position *dpos, bool hex_bl, double hex_lng);				// transfer dot_density to dot_position
// void den2pos_tetgen(dot_density *dden, dot_position *dpos, bool hex_bl, double hex_lng);		// transfer dot_density to dot_position by tetgen software

// // functino for figure
// void gnuplot_2d(plot_2d *plt2d);
// void gnuplot_3d(plot_3d *plt3d);
// void gnuplot_3d_matrix(plot_3d_matrix *plt3dm);
// void gnuplot_3d_vector(plot_3d_vector *plt3dv);

// // function for debug
// void debug_den_to_pos(dot_density *dden, dot_position *dpos);

// main program


int single_proc_main();
//int old_main();

#define MAX_OUTPUT_RAY 5

static unsigned long hit_local_error_count = 0;
static unsigned long iteration_count = 0;


int single_proc_main()
{
	int i;
	// define variables
	opt_mat opm;					// for optic materials
	opt_source ops;					// for optic source
	opt_record opr;					// for recording rays emitted from light guides
	ray_traces rays;				// for samplings of ray tracing
	ray_trace1 ray1;
	local_str lstr;					// for local microstructure
	dot_density dden;				// for global dot density
	dot_position dpos;				// for global dot potision
	char fpname[256];					// for reading parameters
	struct ray_trace1 source_ray[2];
	opt_record_head *opr_head;
	data_file_header opr_data_file_header;
	char child_prefix[1024];

	set_start_time("Total");
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname, NULL);

	// allocate memory
	allocmem_opm(n_wl, n_mat, &opm);			
	allocmem_ops(n_x, n_y, n_z, n_tha, n_phi, xl_or, yl_or, zl_or, xl_rng, yl_rng, zl_rng, &ops);				
	allocmem_rays(n_ray, n_gaus, &rays, &ray1);	
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, &opr);			
	allocmem_local_str(nx_str, ny_str, center_x, center_y, xstr_rng, ystr_rng, &lstr);		
	allocmem_dot_density(nx_den, ny_den, xden_or, yden_or, xden_rng, yden_rng, &dden);	
	allocmem_dot_position(n_dots, hex_bl, hex_lng, &dpos);	

	// program body
		

	set_start_time("gen_source_ray");
	// generate light source rays & initialize microstructure
	gen_source_ray(&ops, &rays);
	set_end_time("gen_source_ray");
	
	set_start_time("read_microstr");
	read_microstr(str_file, &lstr);
	set_end_time("read_microstr");

	// moduel 2...

	pl();
	if (enable_dot_pos_file)
	{
		printf("load dot position from dot pos file: %s\n", dot_pos_file);
		if(strlen(dot_pos_file) > 4 && !strcmp(dot_pos_file + strlen(dot_pos_file) - 4, ".txt"))
		{
			if(!load_dot_position_txt_file(dot_pos_file, &dpos))
			{
				pl();
				system("pause");
				return 1;
			}
		}
		else
		{ 
			if(!load_dot_position_dat_file(dot_pos_file, &dpos))
			{
				pl();
				system("pause");
				return 1;
			}
			part_dots(&dpos);
		}
		// save_dot_position_txt_file("r.dot_position.txt", &dpos);
	}
	else
	{
		//debug test
		set_start_time("debug_den_to_pos");
		debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
		set_end_time("debug_den_to_pos");

		set_start_time("part_dots");

		part_dots(&dpos);
		//save_dot_position_file(&dpos);
		set_end_time("part_dots");
	}
	pl();
	set_start_time("ray tracing");

	opr_head = new_opt_record_head();

	open_ray_csv("ray_log.csv");
	
	printf("ray tracing\n");
	for(i=0; i<n_ray; i++)
	// for(i=0; i<1; i++)
	{
		printf("\r%d/%d", i, n_ray);
		ray1.ngaus 	= 0; 
		ray1.n1 	= 1.0; 
		ray1.n2 	= 1.58;
		

		ray1.xr 	= rays.xr[i]; 
		
		// ray1.yr 	= 0.0; 
		ray1.yr 	= rays.yr[i]; 
		
		ray1.zr 	= rays.zr[i]; 
		
		// ray1.thar 	= 100; 
		ray1.thar 	= rays.thar[i]; 
		
		// ray1.phir 	= 0;
		ray1.phir 	= rays.phir[i];
		// ray1.inty   = 10.0*rays.inty[i];
		ray1.inty   = 10;
		ray1.nx = 0.0;  ray1.ny = 0.0;  ray1.nz = 0.0;
		// dumpRay1(&ray1);
		//dumpRay1toFile(&ray1);
		// ray1.ngaus = 1; ray1.inty = 1.0; ray1.n1 = 1.0; ray1.n2 = 1.0;
		// ray1.xr = 0.0; ray1.yr = 0.0; ray1.zr = 0; 
		// ray1.thar = 100; ray1.phir =0.0;
		get_child_prefix("", child_prefix, i);
		trace_one_ray(child_prefix, &ray1, &dpos, &opr, &lstr);
	
		
	}
	printf("\n");

	close_ray_csv();
	set_end_time("ray tracing");

	strcpy(opr_data_file_header.prefix, "prefix--");
	opr_data_file_header.offset 		= 0;
	opr_data_file_header.entry_size 	= ACTUAL_OPT_RECORD_SIZE;
	opr_data_file_header.count 			= ((glist_head_t*)opr_head)->len;

	
	save_opt_record_txt_file_pos(output_opt_record_txt, &opr);
	// set_start_time("save opt file");
	// save_opt_record_file(output_opt_record, &opr_data_file_header, opr_head);
	// set_end_time("save opt file");

	// dump_opt_record_head(opr_head);
	// free_opt_record_head(&opr_head);

	// opr_head = new_opt_record_head();

	// memset(&opr_data_file_header, 0, sizeof(data_file_header));
	// set_start_time("load opt file");
	// load_opt_record_file(output_opt_record, &opr_data_file_header, opr_head);
	// set_end_time("load opt file");
	// dump_opt_record_head(opr_head);
	// free_opt_record_head(&opr_head);

	// module 5...	

	//deallocate memory
	deallocmem_opm(&opm);
	deallocmem_ops(&ops);
	deallocmem_rays(&rays, &ray1);
	deallocmem_record(&opr);
	deallocmem_local_str(&lstr);
	deallocmem_dot_density(&dden);
	deallocmem_dot_position(&dpos);
	set_end_time("Total");

	print_all_execution_time();

	system("pause");
	return 0;

}

int main(int argc, char** argv)
{

	//return load_matrix();
	 return single_proc_main();
	//return test_main();
	// return test_opt_main();
	//return old_main();
	//return test_merge_main();
}

