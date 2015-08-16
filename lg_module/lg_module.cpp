
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
int old_main();

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
	char child_prefix[256];

	set_start_time("Total");
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname);



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
	//debug test
	set_start_time("debug_den_to_pos");
	debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
	set_end_time("debug_den_to_pos");

	set_start_time("part_dots");
	part_dots(&dpos);
	set_end_time("part_dots");

	set_start_time("ray tracing");

	opr_head = new_opt_record_head();

	open_ray_csv("ray_log.csv");
	for(i=0; i<n_ray; i++)
	// for(i=0; i<1; i++)
	{
		
		ray1.ngaus 	= 1; 
		ray1.n1 	= 1.0; 
		ray1.n2 	= 1.49;
		

		ray1.xr 	= rays.xr[i]; 
		
		// ray1.yr 	= 0.0; 
		ray1.yr 	= rays.yr[i]; 
		
		ray1.zr 	= rays.zr[i]; 
		
		// ray1.thar 	= 100; 
		ray1.thar 	= rays.thar[i]; 
		
		// ray1.phir 	= 0;
		ray1.phir 	= rays.phir[i];
		ray1.inty   = 10.0*rays.inty[i];
		ray1.nx = 0.0;  ray1.ny = 0.0;  ray1.nz = 0.0;
		//dumpRay1(&ray1);
		//dumpRay1toFile(&ray1);
		// ray1.ngaus = 1; ray1.inty = 1.0; ray1.n1 = 1.0; ray1.n2 = 1.0;
		// ray1.xr = 0.0; ray1.yr = 0.0; ray1.zr = 0; 
		// ray1.thar = 100; ray1.phir =0.0;

		get_child_prefix("", child_prefix, i);
		trace_one_ray(child_prefix, &ray1, &dpos, opr_head, &lstr);

		// find_str_hit_global(&ray1, &dpos, &opr);
		// if (!find_str_hit_local(&ray1, &lstr))
		// {
		// 	dumpRay1(&ray1);
		// 	continue;	
		// }
		// CalcMainReflectiveRay(&ray1, &source_ray[0]);
		// CalcMainTransmittanceRay(&ray1, &source_ray[1]);
		// call_CalcGaussScatteredRay(&source_ray[0]);
	}

	close_ray_csv();
	set_end_time("ray tracing");

	strcpy(opr_data_file_header.prefix, "prefix--");
	opr_data_file_header.offset 		= 0;
	opr_data_file_header.entry_size 	= ACTUAL_OPT_RECORD_SIZE;
	opr_data_file_header.count 			= ((glist_head_t*)opr_head)->len;

	
	set_start_time("save opt file");
	save_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);
	set_end_time("save opt file");

	// dump_opt_record_head(opr_head);
	free_opt_record_head(&opr_head);

	opr_head = new_opt_record_head();

	memset(&opr_data_file_header, 0, sizeof(data_file_header));
	set_start_time("load opt file");
	load_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);
	set_end_time("load opt file");
	// dump_opt_record_head(opr_head);
	free_opt_record_head(&opr_head);


	// for(i=0; i<1; i++)
	// {
	// 	// CalcMainReflectiveRay(&incident_ray[i], &source_ray[i]);
	// 	// CalcMainTransmittanceRay(&incident_ray[i], &source_ray[i+100]);

	// 	CalcMainReflectiveRay(&ray1, &source_ray[i]);
	// 	CalcMainTransmittanceRay(&ray1, &source_ray[i+100]);
	// }

	// for(i=0; i<200; i++)
	// {
	// 	call_CalcGaussScatteredRay(&source_ray[i]);
	// }

	pLU(hit_local_error_count);
	pLU(iteration_count);

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
int old_main()
{
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
	int type;

	set_start_time("Total");
	srand((unsigned)time(NULL));	// initiate rand seed 

	// read in setup-parameters
	strcpy(fpname, "parameters.txt");

	read_setup(fpname);



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
	//debug test
	set_start_time("debug_den_to_pos");
	debug_den_to_pos(&dden, &dpos);	// generate dot pattern for test;
	set_end_time("debug_den_to_pos");

	set_start_time("part_dots");
	part_dots(&dpos);
	set_end_time("part_dots");



	set_start_time("find_str_hit_global");
	// input: ray1, dpos, opr
	// output: ray1, opr
	find_str_hit_global(&ray1, &dpos, &opr, &type);
	set_end_time("find_str_hit_global");

	// moduel 3...
	// debug test

	ray1.ngaus = 1; ray1.inty = 1.0; ray1.n1 = 1.0; ray1.n2 = 1.0;
	ray1.xr = 0.0; ray1.yr = 0.0; ray1.zr = 0; 
	ray1.thar = 100; ray1.phir =0.0;


	printf("\n incident pos in microstructure box: %lf\t%lf\t%lf\n",ray1.xr, ray1.yr, ray1.zr);
	
	set_start_time("find_str_hit_local");
	find_str_hit_local(&ray1, &lstr);	
	set_end_time("find_str_hit_local");
	printf(" hit pos in microstructure box: %lf\t%lf\t%lf\n",ray1.xr, ray1.yr, ray1.zr);

	// moduel 4...
	// debug test

	int i;
	// struct ray_trace1 incident_ray[100];    // incident ray source
	// struct ray_trace1 source_ray[200];      // output ray sources includes Reflective and Transmittance ray


	struct ray_trace1 source_ray[2];

	set_start_time("Module_IV");
	for(i=0; i<n_ray; i++)
	{
		CalcMainReflectiveRay(&ray1, &source_ray[0]);
		CalcMainTransmittanceRay(&ray1, &source_ray[1]);
		call_CalcGaussScatteredRay(&source_ray[0]);
	}
	set_end_time("Module_IV");

	// for(i=0; i<1; i++)
	// {
	// 	// CalcMainReflectiveRay(&incident_ray[i], &source_ray[i]);
	// 	// CalcMainTransmittanceRay(&incident_ray[i], &source_ray[i+100]);

	// 	CalcMainReflectiveRay(&ray1, &source_ray[i]);
	// 	CalcMainTransmittanceRay(&ray1, &source_ray[i+100]);
	// }

	// for(i=0; i<200; i++)
	// {
	// 	call_CalcGaussScatteredRay(&source_ray[i]);
	// }

	pLU(gaussScatteredRaycount);

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

int test_merge_main()
{
	int i;
	char dest_file[100];
	char *opt_record_name[4];

	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    char prefix[100];
	
	sprintf(prefix, "_lg_module_%d_%d_%d_%d_%d_%d\n", 
		now->tm_year+1900,
		now->tm_mon+1,
		now->tm_mday,
		now->tm_hour,
		now->tm_min,
		now->tm_sec
	);

	pStr(prefix);
	strcpy(dest_file, prefix);
	

	for(i=0; i<4; i++)
	{
		opt_record_name[i] = (char*)malloc(100);
		sprintf(opt_record_name[i], "%s_%d", prefix, i);
	}

	merge_opt_record_files(prefix, dest_file, 4, (const char**)opt_record_name);

	return 0;
}
int test_main()
{

	opt_record *opr;					// for recording rays emitted from light guides
	opt_record_head *opr_head;
	data_file_header opr_data_file_header;
	
	char parameters[] = "parameters.txt";
	read_setup(parameters);

	// opr_head = new opt_record_head;

	opr_head = new_opt_record_head();
	opr = new opt_record;
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, opr);
	opr->x0 = 1.1;
	opr->y0 = 2.1;
	opr->z0 = 3.1;

	
	add_opt_record(opr_head, opr);

	opr = new opt_record;
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, opr);
	opr->x0 = 1.2;
	opr->y0 = 2.2;
	opr->z0 = 3.2;

	
	add_opt_record(opr_head, opr);


// typedef struct _data_file_header
// {
// 	char prefix[256];
// 	unsigned int offset;
// 	int entry_size;
// 	long int count;
// }data_file_header;

	strcpy(opr_data_file_header.prefix, "prefix--");
	opr_data_file_header.offset 		= 0;
	opr_data_file_header.entry_size 	= ACTUAL_OPT_RECORD_SIZE;
	opr_data_file_header.count 			= 2;


	dump_opt_record_head(opr_head);

	save_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);

	free_opt_record_head(&opr_head);

	opr_head = new_opt_record_head();

	memset(&opr_data_file_header, 0, sizeof(data_file_header));
	load_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);
	dump_opt_record_head(opr_head);
	free_opt_record_head(&opr_head);


	return 0;	
}

int test_opt_main()
{
	opt_record *opr;					// for recording rays emitted from light guides
	opt_record_head *opr_head;
	data_file_header opr_data_file_header;
	opt_record_data *opr_data;
	glist_t *cur;
	
	char parameters[] = "parameters.txt";
	read_setup(parameters);

	int i;
	int opr_size;
	int array_size;
	int entry_size;

	// opr_head = new opt_record_head;

	opr_size 	= sizeof(struct opt_record) - sizeof(double*);
	array_size 	= sizeof(double)*nx_rcd*ny_rcd*ntha_rcd*nphi_rcd;
	entry_size 	= opr_size + array_size;

	opr_data = (opt_record_data*)malloc(entry_size);

	opr_head = new_opt_record_head();
	opr 	 = new opt_record;

	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, opr);
	opr->x0 = 1.1;
	opr->y0 = 2.1;
	opr->z0 = 3.1;

	
	add_opt_record(opr_head, opr);
	
	copy_opr_to_opr_data(opr_data, opr, array_size);
	// dump_opt_record_data(opr_data);

	opr = new opt_record;
	allocmem_record(nx_rcd, ny_rcd, ntha_rcd, nphi_rcd, xrcd_or, yrcd_or, zrcd_or, xrcd_rng, yrcd_rng, opr);
	opr->x0 = 1.2;
	opr->y0 = 2.2;
	opr->z0 = 3.2;


	add_opt_record(opr_head, opr);
	
	copy_opr_to_opr_data(opr_data, opr, array_size);
	// dump_opt_record_data(opr_data);


	cur = ((glist_head_t*)opr_head)->child;
	
	for(i=0; i<2 && cur != NULL; i++)
	{
		opr = (opt_record*)cur->vptr;
		copy_opr_to_opr_data(opr_data, opr, array_size);
		dump_opt_record_data(opr_data);		
		
		cur = cur->next;
	}

	strcpy(opr_data_file_header.prefix, "prefix--");
	opr_data_file_header.offset 		= 0;
	opr_data_file_header.entry_size 	= entry_size;
	opr_data_file_header.count 			= 2;


	dump_opt_record_head(opr_head);

	save_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);
	free_opt_record_head(&opr_head);
	opr_head = new_opt_record_head();
	memset(&opr_data_file_header, 0, sizeof(data_file_header));
	load_opt_record_file("aaa.opt_recoder", &opr_data_file_header, opr_head);
	dump_opt_record_head(opr_head);
	free_opt_record_head(&opr_head);

	return 0;

}

int main(int argc, char** argv)
{

	return single_proc_main();
	//return test_main();
	// return test_opt_main();
	//return old_main();
	//return test_merge_main();
}

