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
#include "mpi.h"
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
double myrand(double *a, double *b, double L, int N, double r){
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
  int maxarg = 9;
  if(argc != maxarg){
    cout << argc << "Incorrect inputs. Try something like:\n ./myfunc.exe -L 0.1 -r 3.2 -x 50 -iter 1000";
  }
  else{
    double L, r;
    string c;
    int i, j, xrng, iter;
    int ierr, numProcs, my_rank, length;
    char proc_name[MPI_MAX_PROCESSOR_NAME];

    //parse cmdln args
    for (i = 1; i < maxarg; i+=2){
      c = argv[i];
      if(c == "-L")		
	L = atof(argv[i+1]);
      if(c == "-r")
	r = atof(argv[i+1]);
      if(c == "-x")
	xrng = atof(argv[i+1]);    // number of initial conditions in x
      if(c == "-iter")
	iter = atof(argv[i+1]);
    }

    /*    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	*/
	
    //calculate random function
    int N = 10/L;    //number of fourier modes    
    double a[N], b[N], x[xrng], p[iter];    // allocate space for the random numbers
    double * aptr, * bptr, * xptr, * pptr;    // pointer to arrays
    aptr = a;
    bptr = b;
    xptr = x;
    pptr = p;
    srand (time(NULL));   // set the random seed to current time	
    myrand(aptr,bptr,L,N,r);  //populate the arrays with random numbers

    // check output of random coefficients
    for(i = 0; i < N; i++){
      cout << i << ": "<< a[i] << " " << b[i]<< "\n";
    }

    // check output of R(x)
    double h = 1.0/xrng;    // h same for all procs

    // initialize the x0 array
    for (i = 0; i < xrng; i++){
      xptr[i] = i*h;
    }

    // begin fixed point iteration on the start position x[0]
    p[0] = x[0];
    for (i = 0; i < iter-1; i++){
      pptr[i+1] = R(pptr[i], aptr, bptr, r, N) * pptr[i] * ( 1 - pptr[i] );
    }

    for (i = 0; i < xrng; i++){
      cout << "i: " << i << " xvalue: " << p[i] << "\n";
    }
    //    ierr = MPI_Finalize();	
    return 0;
  }
}
