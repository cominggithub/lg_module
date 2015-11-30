#ifndef _VAR_DEF_H_
#define _VAR_DEF_H_
// all length-unit is mm, and all angle-unit is degree (0-180)

// general constants
extern double pi;
extern double delta;

// light-guide geometry (mm)
extern double xdim, ydim, zdim_in, zdim_out, z_reflector;					// zdim_out is decided by zdim_in + heightness of microstructure


// optical material
extern long int n_wl;											// n_wl: number of wavelength; <default value>
extern long int n_mat;											// n_material: number of optical materials; <default value>
extern double air_indx, guide_indx, refl_indx;	// material indexes = 0 ~ (n_material-1) corresponding to n_material;

// light source
extern long int n_x, n_y, n_z;						// grid number along x, y, and z axes; <default value>
extern long int n_tha, n_phi;						// grid number along theta and phi; <default value>
extern double xl_or, yl_or, zl_or;					// origin of the coordinate
extern double xl_rng, yl_rng, zl_rng;				// dimensions along x, y, and z directions
extern double z_reflector;
// ray tracing
extern long int n_ray;							// number of sampling rays
extern long int n_gaus;							// number of gaussin-scattering rays;
extern double ray_threshold;					// ignored ray if ray-intensity is below this threshold value

// lguide performance (measurement)
extern long int nx_rcd, ny_rcd;								// grid number along x, y, and z axes for records; <default value>
extern long int ntha_rcd, nphi_rcd;							// grid number along theta and phi for records; <default value>
extern double xrcd_or, yrcd_or, zrcd_or;						// origin of the coordinate
extern double xrcd_rng, yrcd_rng;								// dimensions along x, y, and z directions

// local structure
extern long int nx_str, ny_str;						// number of grids along x and y
extern double center_x, center_y;						// center of the microstructure
extern double xstr_rng, ystr_rng;						// dimensions along x and y
extern char str_file[];

// global dot density
extern long int nx_den, ny_den;								// grid number along x and y axes for records;
extern double den_2_pos_scale;
extern double xden_or, yden_or;								// origin of the coordinate
extern double xden_rng, yden_rng;								// dimensions along x, y, and z directions

// global dot spatial pattern
extern long int n_dots;											// total dots
extern long int partn_x, partn_y;
extern char hex_bl;													// define whether fit to hexagon 
extern double hex_lng;

// local variables for Module IV
extern double ThetaOne;               // The variable for saving ThetaOne.
extern double ThetaTwo;               // The variable for saving ThetaTwo.
extern double theta_st;               // The angale between main ray and scattered ray
extern double ReflectiveRayIntensity; // The variable for saving the intensity of ReflectiveRay.
                                      // It will be used between CalcMainReflectiveRay() and CalcMainTransmittanceRay()
extern double IntensityThreshold;     // The threshold for determine the intensity of ray
									// define the lattice constant of hexagon

// debug variable
extern bool enable_gnu_plot;
extern unsigned long gaussScatteredRaycount;

// output file name
extern char output_opt_record_dat[];
extern char output_opt_record_txt[];
extern char output_dir[];
extern char input_script[];
extern char output_data2d_txt[];
extern char output_data3d_txt[];
extern char output_data3dm_txt[];
extern char output_data3dv_txt[];
#endif