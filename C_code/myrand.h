#ifndef myrand_h
#define myrand_h

double rand_draw(double a, double b);

/*calculate random coefficients*/
double myrand(double *a, double *b, double L, int N, double r);
//a b are pointers to an array 

double myrand(double *a, double *b, double L, int N, double r);
// a b are pointers to an array

double R(double x, double *a, double *b, double r, int N);
//a b are pointers
#endif