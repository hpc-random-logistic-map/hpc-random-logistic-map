#include "orbit.h"
#include <stdlib.h> // size_t
#include "myrand.h"

void prob_of_orbits(double L,double r,double tmp, size_t xlen,size_t N, double tol)
{
	double N = 10. / L;
	size_t xlen = 57;
	size_t iter= 1000;
	size_t xrng = 20;
	int kmax = 1;
	size_t maxp = xlen - 5;
	double st[xrng];
	st[0] = tol;
	double gap = 1.0 / double(xrng - 1);
	for (int i = 1; i < xrng;i++)
		st[i] = st[i - 1] + gap;
	double *y[maxp];
	for (int i = 0; i < maxp; i++)
	{
		y[i] = new double[2];
		for (int j = 0; j < 2; j++)
			y[i][j] = 0;
	}
	// myrand need to be check with others 
	double * a, b;
	myrand(a,b,L, N, r);
	
	double orbits[maxp][maxp + 1];
	for (int i = 0; i < maxp; i++)
	{
		for (int j = 0; j < maxp+1; j++)
			orbits[i][j] = -1.0;
	}

	int u = 1;
	double *xv;
	for (int j = 0; j < xrng; j++)
	{
		xv = cobweb(st[j, iter, a, b, r]);
		//since C++ return 1 pointer only,extra work needed to be done to calculate the size 
		//trick is allocate an int size in the xv to store the size then we can read the size from the array
		//this trick need to be done in cobweb to prevent extra unnecessary work here 
		//end = sizeof(xv)-1;
		for (int w = 0; w < maxp; w++)
		{
			if (abs(end - w) - xv(end)) < tol
			{
			orbits(u, 0) = w;
			orbits(u, 1) = xv(end);

			for (int v = 2; v < w + 1; v++)
				orbits(u, v) = xv(end - v + 1);
			}
			u = u + 1;
			break;
		}
	}
	double **u_orbits = uorbits(orbits);
	//end = sizeof(u_orbits)
	double num_periods[end];
	for (int i = 0; i < end;i++)
		num_periods[i] = u_orbits(i, 1);
	int count;
	for (int w = 0; w < maxp; w++)
	{
		y(w, 0) = w;
		count = 0;
		for (int k = 0; k < end;k++)
		if (num_periods[k] == w)
			count++;
		y(w, 2) = count;

	}

	double p[maxp][2];
	for (int i = 0; i < maxp; i++)
	{
		for (int j = 0; j < 2; j++)
			p[i][j] = 0;
	}
	
	double total = 0;
	for (int i = 0; i < maxp; i++)
	{

		p[i][2] = (y[i][0] + y[i][1]);
		total += p[i][2];
		p[i][1] = (y[i][0] + y[i][1]) / 2;
	
	}
	
	double probs[maxp];
	for (int i = 0; i < maxp; i++)
	{
		probs[i] = p(i, 2) / total;
	}

	//writing data to HDF5 instead of CSV
}