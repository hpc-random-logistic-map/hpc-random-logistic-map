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
using namespace std;

/*Produce a random number in the range [a,b]*/
double rand_draw(double a, double b) {
    double random = ((float) rand()) / (float) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

/*calculate random coefficients*/
double myrand(double *a,double *b,double L,int N, double r){
    int i;
    double sigma = (log(4.0/r) * tanh(L/4.0) ) / ( sqrt(1.5*tanh(0.5*L)) );
    double alpha = sigma*sigma*tanh(0.5*L);
    double S;
    float Mn;
    for(i = 0; i < N; i++){
	S = alpha * exp(-L * abs(i));
	Mn = sqrt(1.5*S);
	a[i] = rand_draw(-Mn,Mn);
	b[i] = rand_draw(-Mn,Mn);
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
	cout << argc << "Incorrect inputs. Try something like:\n ./myfunc.exe -L 0.1 -r 3.2";
    }
    else{
	double L, r;
	string c;
	int i, j;
	int ierr, numProcs, my_rank, length;
	char proc_name[MPI_MAX_PROCESSOR_NAME];

	//parse cmdln args
	for (i = 1; i < maxarg; i+=2){
	    c = argv[i];
	    if(c == "-L")		
		L = atof(argv[i+1]);
	    if(c == "-r")
		r = atof(argv[i+1]);
	}

	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	
	
	//calculate random function
	int N = 10/L;    //number of fourier modes
	int xrng = 50;    // number of initial conditions in x
	double a[N], b[N], x[xrng];    // allocate space for the random numbers
	double * aptr, * bptr, * xptr;    // pointer to arrays
	aptr = a;
	bptr = b;
	xptr = x;
	srand (time(NULL));   // set the random seed to current time	
	myrand(aptr,bptr,L,N,r);  //populate the arrays with random numbers

	//begin mpi
	double h = 1.0/xrng;    // h same for all procs
	double mysum = 0.0;
	double fs = 0.0;
	double tol = 10e-6;
	//reduction on mysum
	while (x < 1){
	for(j = 0; j < N; j++){
	    fs = 2*a[j]*cos(2*M_PI*j*x) - b[j]*sin(2*M_PI*j*x);
	    mysum = mysum + fs;
	}
	x = x + tol;
	}

	double xi = log(r) + mysum;
	double y = exp(xi);
	return y;

	for(i = 0; i < N; i++){
	    cout << i << ": "<< a[i] << " " << b[i]<< "\n";
	}
    }
}
