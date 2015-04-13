
void find_str_hit_global(ray_trace1 *ray1)  // change coordinate origin into the center of the top plane of the box containg a microstrcuture
{

}

void part_dots(dot_position *dpos)
{
	long int i, j, nx, ny, indi, indbuf;
	double dx, dy, xbuf, ybuf;

	nx = dpos->partnx;
	ny =  dpos->partny;
	dx = 1.0*xdim/nx+delta;
	dy = 1.0*ydim/ny+delta;

	for(i=0; i<dpos->ndot; i++)
	{
		indi =	int(dpos->xd[i]/dx)*ny+int(dpos->yd[i]/dy);			// the index corresponding to partition array
		dpos->partindx[i] = indi;
	}
	//sort xd/yd by partindx
	for(i=0; i<dpos->ndot; i++)
	{
		for(j=i+1; j<dpos->ndot; j++)
		{
			if(dpos->partindx[i]>dpos->partindx[j])
			{
				indbuf = dpos->partindx[i];
				dpos->partindx[i] = dpos->partindx[j];
				dpos->partindx[j] = indbuf;

				xbuf = dpos->xd[i];
				dpos->xd[i] = dpos->xd[j];
				dpos->xd[j] = xbuf;

				ybuf = dpos->yd[i];
				dpos->yd[i] = dpos->yd[j];
				dpos->yd[j] = ybuf;
			}
		}
	}
	for(i=0; i<dpos->ndot; i++)
	{
			
	}
}