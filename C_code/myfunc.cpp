/*code to generate random noise*/
#include <iostream> // cout
#include <string>   //string
#include <fstream> //write to csv; ofstream
#include <cstring>
#include <stdlib.h>    // atof, atoi, itoa
#include <sys/time.h>    /*for get_wall_time*/
#include <math.h>    /*for pow*/
#include <cmath> /*for abs*/
#include <stdio.h> //printf
//#include "mpi.h"
#include <limits> //long int for 10^9 runs
#include <random> //for uniform random distribution
using namespace std;

/*calculate random coefficients*/
double myrand(double *a,double *b,double L,int N, double r){
    int i;
    double sigma = (log(4.0/r) * tanh(L/4.0) ) / ( sqrt(1.5*tanh(0.5*L)) );
    double alpha = sigma*sigma*tanh(0.5*L);
    double S;
    double Mn;
    default_random_engine generator;
    uniform_real_distribution<double> distribution(-Mn,Mn);
    for(i = 0; i < N; i++){
	S = alpha * exp(-L * abs(i));
	Mn = sqrt(1.5*S);
	a[i] = 1;
    }

    return 0;
}

/*calculate random function at location x*/
double R(double x, double *a, double *b, double r, int N){
    double mysum = 0.0;
    double fs = 0.0;
    
    //reduction on mysum
    for(int j = 0; j < N; j++){
	fs = 2*a[j]*cos(2*M_PI*j*x) - b[j]*sin(2*M_PI*j*x);
	mysum = mysum + fs;
    }

    double xi = log(r) + mysum;
    double y = exp(xi);
    return y;
}

int main(int argc, char* argv[]){
    int maxarg = 5;
    if(argc != maxarg){
	cout << argc << "Incorrect inputs. \n";
    }
    else{
	double L, r;
	string c;
	int i;

	//parse cmdln args
	for (i = 1; i < maxarg; i+=2){
	    c = argv[i];
	    if(c == "-L")		
		L = atof(argv[i+1]);
	    if(c == "-r")
		r = atof(argv[i+1]);
	}

	//calculate random function
	int N = 10/L;    //number of fourier modes
	double a[N], b[N];    //allocate space for the random numbers
	double * aptr, * bptr;    //pointer to arrays
	aptr = a;
	bptr = b;
	myrand(aptr,bptr,L,N,r);
	for(i = 0; i < N; i++){
	    cout << a[i] << " ";
	}
    }
}
