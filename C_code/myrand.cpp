#include "myrand.h"


/*Produce a random number in the range [a,b]*/
double rand_draw(double a, double b) {
	double random = ((float) rand()) / (float) RAND_MAX;
	double diff = b - a;
	double r = random * diff;
	return a + r;
}

/*calculate random coefficients*/
void myrand(double *a, double *b, double L, int N, double r){
	int i;
	double sigma = (log(4.0 / r) * tanh(L / 4.0)) / (sqrt(1.5*tanh(0.5*L)));
	double alpha = sigma*sigma*tanh(0.5*L);
	double S;
	float Mn;
	for (i = 0; i < N; i++){
		S = alpha * exp(-L * abs(i));
		Mn = sqrt(1.5*S);
		a[i] = rand_draw(-Mn, Mn);
		b[i] = rand_draw(-Mn, Mn);
	}
	//return 0;
}

/*calculate random function at location x*/
double R(double x, double *a, double *b, double r, int N){
	double mysum = 0.0;
	double fs = 0.0;

	//reduction on mysum
	for (int j = 0; j < N; j++){
		fs = 2 * a[j] * cos(2 * M_PI*j*x) - b[j] * sin(2 * M_PI*j*x);
		mysum = mysum + fs;
	}

	double xi = log(r) + mysum;
	double y = exp(xi);
	return y;
}