#include <stdio.h>
#include <stdlib.h>

#include "plt_figure.h"
#include "var_def.h"
#include "dbg_log.h"
void gnuplot_2d(plot_2d *plt2d)
{
	char cmd[256];
	char output_name[256];
	char script_name[256];

	if (!enable_gnu_plot)
		return;
	FILE *script, *data;
	long int i;

	getTmpFileFullPath(output_name, "data2d.txt");
	getTmpFileFullPath(script_name, "script");
	// save data
	// data = fopen("data2d.txt","w");
	data = fopen(output_name,"w");
	for(i=0;i<plt2d->npnt;i++)
	{
		fprintf(data,"%16.7f\t%16.7f\n",plt2d->x[i],plt2d->y[i]);
	}
	fclose(data);
	// save script for gnupolt
	script = fopen(script_name,"w");
	// fprintf(script,"plot \"data2d.txt\" with points pointtype 7 pointsize 0.2 \n");
	fprintf(script,"plot \"%s\" with points pointtype 7 pointsize 0.2 \n", output_name);
	fprintf(script,"pause -1 \"press RETURN to continue\"\n");
	fclose(script);
	// plot
	sprintf(cmd, "gnuplot \"%s\"", script_name);
	system(cmd);
	return;
}
void gnuplot_3d(plot_3d *plt3d)
{
	char cmd[256];
	char output_name[256];
	char script_name[256];

	if (!enable_gnu_plot)
		return;
	FILE *script, *data;
	long int i;

	getTmpFileFullPath(output_name, "data3d.txt");
	getTmpFileFullPath(script_name, "script");

	// save data
	// data = fopen("data3d.txt","w");
	data = fopen(output_name,"w");
	for(i=0;i<plt3d->npnt;i++)
	{
		fprintf(data,"%16.7f\t%16.7f\t%16.7f\n",plt3d->x[i],plt3d->y[i],plt3d->z[i]);
	}
	fclose(data);
	// save script for gnupolt
	script = fopen(script_name,"w");
	// fprintf(script,"splot \"data3d.txt\" with points pointtype 7 pointsize 0.2 \n");
	fprintf(script,"splot \"%s\" with points pointtype 7 pointsize 0.2 \n", output_name);

	fprintf(script,"pause -1 \"press RETURN to continue\"\n");
	fclose(script);
	// plot
	sprintf(cmd, "gnuplot \"%s\"", script_name);
	system(cmd);
	return;
}
void gnuplot_3d_matrix(plot_3d_matrix *plt3dm)
{
	char cmd[256];
	char output_name[256];
	char script_name[256];

	if (!enable_gnu_plot)
		return;
	FILE *script, *data;
	long int i, j, nx, ny;

	getTmpFileFullPath(output_name, "data3dm.txt");
	getTmpFileFullPath(script_name, "script");

	// save data
	// data = fopen("data3dm.txt","w");

	data = fopen(output_name,"w");
	nx = plt3dm->nx;
	ny = plt3dm->ny;
	for(j=0;j<ny;j++)
	{
		for(i=0;i<nx;i++){	fprintf(data,"%16.7f\t",plt3dm->v[i*ny+j]);	}
		fprintf(data,"\n");
	}
	fclose(data);
	// save script for gnupolt
	// script = fopen("script","w");
	script = fopen(script_name,"w");
	fprintf(script,"set pm3d \n");
	fprintf(script,"unset surface \n");
	// fprintf(script,"splot \"data3dm.txt\" matrix \n");
	fprintf(script,"splot \"%s\" matrix \n", output_name);
	fprintf(script,"pause -1 \"press RETURN to continue\"\n");
	fclose(script);
	// plot
	sprintf(cmd, "gnuplot \"%s\"", script_name);
	system(cmd);
	return;
}
void gnuplot_3d_vector(plot_3d_vector *plt3dv)
{
	char cmd[256];
	char output_name[256];
	char script_name[256];

	if (!enable_gnu_plot)
		return;
	FILE *script, *data;
	long int j, npnt;

	getTmpFileFullPath(output_name, "data3dv.txt");
	getTmpFileFullPath(script_name, "script");

	// save data
	// data = fopen("data3dv.txt","w");
	data = fopen(output_name,"w");
	npnt = plt3dv->npnt;
	for(j=0;j<npnt;j++)
	{
		fprintf(data,"%16.7f\t%16.7f\t%16.7f\t%16.7f\t%16.7f\t%16.7f\n",plt3dv->x[j],plt3dv->y[j],\
					plt3dv->z[j],plt3dv->vx[j],plt3dv->vy[j],plt3dv->vz[j]);
	}
	fclose(data);
	// save script for gnupolt
	script = fopen(script_name,"w");
	fprintf(script,"splot \"%s\" with vectors \n", output_name);
	fprintf(script,"pause -1 \"press RETURN to continue\"\n");
	fclose(script);
	// plot
	sprintf(cmd, "gnuplot \"%s\"", script_name);
	system(cmd);
	return;
}
