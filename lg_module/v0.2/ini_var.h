
void read_setup(char *fpname)
{
	int err;
	
	err = conf_load(fpname);
	if(err == 0) 
	{
		printf("parameter file <%s> is not found!\n",fpname);
		exit(0);
	}
	// for light-guid geometry
	xdim = conf_getVarDouble("xdim");
	ydim = conf_getVarDouble("ydim");
	zdim_in = conf_getVarDouble("zdim_in");

	// for optical materials
	n_wl = conf_getVarInt("n_wl");
	n_mat = conf_getVarInt("n_mat");
	air_indx = conf_getVarInt("air_indx");
	guide_indx = conf_getVarInt("guide_indx");
	refl_indx = conf_getVarInt("refl_indx");

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

	// for lguide performance (record/measurement)
	nx_rcd = conf_getVarInt("nx_rcd");
	ny_rcd = conf_getVarInt("ny_rcd");
	ntha_rcd = conf_getVarInt("ntha_rcd");
	nphi_rcd = conf_getVarInt("nphi_rcd");
	xrcd_or = conf_getVarDouble("xrcd_or");
	yrcd_or = conf_getVarDouble("yrcd_or");
	zrcd_or = conf_getVarDouble("zrcd_or");
	xrcd_rng = conf_getVarDouble("xrcd_rng");
	yrcd_rng = conf_getVarDouble("yrcd_rng");
	zrcd_rng = conf_getVarDouble("zrcd_rng");

	// for local structure
	nx_str = conf_getVarInt("nx_str");
	ny_str = conf_getVarInt("ny_str");
	center_x = conf_getVarDouble("center_x");
	center_y = conf_getVarDouble("center_y");
	xstr_rng = conf_getVarDouble("xstr_rng");
	ystr_rng = conf_getVarDouble("ystr_rng");

	// for global dot density
	nx_den = conf_getVarInt("nx_den");
	ny_den = conf_getVarInt("nx_den");
	xden_or = conf_getVarDouble("xden_or");
	yden_or = conf_getVarDouble("yden_or");
	xden_rng = conf_getVarDouble("xden_rng");
	yden_rng = conf_getVarDouble("yden_rng");

	// for dot spatial pattern
	n_dots = conf_getVarInt("n_dots");
	hex_bl = *conf_getVarStr("hex_bl");
	hex_lng = conf_getVarDouble("hex_lng");

}