#include <stdio.h>
#include <stdlib.h>

#include "ini_var.h"
#include "conf_util.h"
#include "var_def.h"
#include <string.h>
#include "dbg_log.h"


bool read_setup(const char *fpname, const char *out_dir)
{
	int err;

	err = conf_load(fpname);
	if(err == 0)
	{
		printf("parameter file %s is not found!\n",fpname);
		return false;
	}
	// for light-guid geometry
	xdim = conf_getVarDouble("xdim");
	ydim = conf_getVarDouble("ydim");
	zdim_in = conf_getVarDouble("zdim_in");

	// for optical materials
	n_wl = conf_getVarInt("n_wl");
	n_mat = conf_getVarInt("n_mat");
	air_indx = conf_getVarDouble("air_indx");
	guide_indx = conf_getVarDouble("guide_indx");
	refl_indx = conf_getVarDouble("refl_indx");

	// for light source
	n_x = conf_getVarInt("n_x");
	n_y = conf_getVarInt("n_y");
	n_z = conf_getVarInt("n_z");
	n_tha = conf_getVarInt("n_tha");
	n_phi = conf_getVarInt("n_phi");
	xl_or = conf_getVarDouble("xl_or");
	yl_or = conf_getVarDouble("yl_or");
	zl_or = conf_getVarDouble("zl_or");
	xl_rng = conf_getVarDouble("xl_rng");
	yl_rng = conf_getVarDouble("yl_rng");
	zl_rng = conf_getVarDouble("zl_rng");

	// for ray tracing
	n_ray = conf_getVarInt("n_ray");
	n_gaus = conf_getVarInt("n_gaus");
	ray_threshold =  conf_getVarDouble("ray_threshold");

	// for lguide performance (record/measurement)
	nx_rcd = conf_getVarInt("nx_rcd");
	ny_rcd = conf_getVarInt("ny_rcd");
	ntha_rcd = conf_getVarInt("ntha_rcd");
	nphi_rcd = conf_getVarInt("nphi_rcd");
	xrcd_or = 0.0;
	yrcd_or = 0.0;
	zrcd_or = 0.0;
	xrcd_rng = xdim;
	yrcd_rng = ydim;

	// for local structure
	nx_str = conf_getVarInt("nx_str");
	ny_str = conf_getVarInt("ny_str");
	center_x = conf_getVarDouble("center_x");
	center_y = conf_getVarDouble("center_y");
	xstr_rng = conf_getVarDouble("xstr_rng");
	ystr_rng = conf_getVarDouble("ystr_rng");
	strcpy(str_file, conf_getVarStr("str_file"));

	// for global dot density
	nx_den = conf_getVarInt("nx_den");
	ny_den = conf_getVarInt("nx_den");
	xden_or = conf_getVarDouble("xden_or");
	yden_or = conf_getVarDouble("yden_or");
	den_2_pos_scale = conf_getVarDouble("den_2_pos_scale");
	xden_rng = xdim;
	yden_rng = ydim;

	// for dot spatial pattern
	n_dots = conf_getVarInt("n_dots");
	partn_x = conf_getVarInt("partn_x");
	partn_y = conf_getVarInt("partn_y");
	hex_bl = *conf_getVarStr("hex_bl");
	hex_lng = conf_getVarDouble("hex_lng");


	enable_gnu_plot = conf_getVarInt("enable_gnu_plot")>0;
	enable_ray_index_log = conf_getVarInt("enable_ray_index_log")>0;

	// read output file name

	strcpy(output_opt_record_dat, conf_getVarStr("output_opt_record_dat"));
	strcpy(output_opt_record_txt, conf_getVarStr("output_opt_record_txt"));

	// strcpy(output_data2d_txt, conf_getVarStr("output_data2d_txt"));
	// strcpy(output_data3d_txt, conf_getVarStr("output_data3d_txt"));
	// strcpy(output_data3dm_txt, conf_getVarStr("output_data3dm_txt"));
	// strcpy(output_data3dv_txt, conf_getVarStr("output_data3dv_txt"));
	// strcpy(input_script, conf_getVarStr("input_script"));
	// strcpy(input_dot_density_txt, conf_getVarStr("input_dot_density_txt"));
	// strcpy(input_microstr_txt, conf_getVarStr("input_microstr_txt"));
	
	if (out_dir != NULL && strlen(out_dir) > 0)
	{
		strcpy(output_dir, out_dir);
	}

	// if (output_dir != NULL)
	// {
	// 	sprintf(input_script, "%s/%s", output_dir, conf_getVarStr("input_script"));
	// 	sprintf(input_dot_density_txt, "%s/%s", output_dir, conf_getVarStr("input_dot_density_txt"));
	// 	sprintf(input_microstr_txt, "%s/%s", output_dir, conf_getVarStr("input_microstr_txt"));
	// 	sprintf(output_data2d_txt, "%s/%s", output_dir, conf_getVarStr("output_data2d_txt"));
	// 	sprintf(output_data3d_txt, "%s/%s", output_dir, conf_getVarStr("output_data3d_txt"));
	// 	sprintf(output_data3dm_txt, "%s/%s", output_dir, conf_getVarStr("output_data3dm_txt"));
	// 	sprintf(output_data3dv_txt, "%s/%s", output_dir, conf_getVarStr("output_data3dv_txt"));
	// }

	return true;
}

void setTmpOutputFolder(const char* folder)
{
	if (folder != NULL && strlen(folder) > 0)
	{
		strcpy(tmp_output_dir, folder);
	}
}


