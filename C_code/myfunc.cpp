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

/*
cobweb aka fixed point iteration
 - x0 is the initial condition
 - iter is the max number of iterates to carry out
 - *xv is the pointer to the result array (length = iter)
 - *a is the pointer to the random values array a
 - *b is the pointer to the random values array b
 - r is the simulation parameter
 - N is the max number of Fourier modes
*/
void cobweb(double x0, int iter, double * xv, double *a, double*b, double r, int N){
    // begin fixed point iteration on the start position x0
  xv[0] = x0;
  for (int i = 0; i < iter-1; i++){
    xv[i+1] = R(xv[i], a, b, r, N) * xv[i] * ( 1 - xv[i] );
  }
}

int main(int argc, char* argv[]){
  int maxarg = 9;
  if(argc != maxarg){
    cout << argc << "Incorrect inputs. Try something like:\n ./myfunc.exe -L 0.1 -r 3.2 -x0 0.5 -iter 1000";
  }
  else{
    double L, r, x0;
    string c;
    int i, j, xrng, iter;

    for (i = 1; i < maxarg; i+=2){
      c = argv[i];
      if(c == "-L"){
	L = atof(argv[i+1]);   // sim param L in [0,1]
      }
      if(c == "-r"){
	r = atof(argv[i+1]);    // sim param r in [0,4]
      }
      if(c == "-x0"){
	x0 = atof(argv[i+1]);    // initial condition x0
      }
      if(c == "-iter"){
	iter = atoi(argv[i+1]);    // number of iterations
      }
    }
    	
    //calculate random function
    int N = 10/L;    //number of fourier modes    
    double * a[N], * b[N], * xv[iter];    // allocate space for the random nums
    srand (time(NULL));   // set the random seed to current time	
    myrand(a,b,L,N,r);  //populate the arrays with random numbers

    // initialize the xv result array with 100 elements
    cobweb(x0, 100, xv, a, b, r, N);

    // call period check to examine

    // continue iterating if period check is false

 
    // check results
    for (i = 0; i < xrng; i++){
      cout << "i: " << i << " xvalue: " << p[i] << "\n";
    }
    return 0;
  }
}
