/*
code to generate random noise
input: 
- parameter: L, correlation length
- parameter: r
output:
- csv file with two columns for a and b
- length of file depends on input (number of fourier modes)
*/
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

int main(int argc, char* argv[]){
  int maxarg = 5;
  if(argc != maxarg){
    cout << argc << "Incorrect inputs. Try something like:\n ./generate_rands.exe -L 0.1 -r 3.2";
  }
  else{
    double L, r, x0;
    string c;

    for (int i = 1; i < maxarg; i+=2){
      c = argv[i];
      if(c == "-L"){
	L = atof(argv[i+1]);   // sim param L in [0,1]
      }
      if(c == "-r"){
	r = atof(argv[i+1]);    // sim param r in [0,4]
      }
    }
    	
    //calculate random function
    int N = 10/L;    //number of fourier modes    
    double a[N], b[N];
    double *ap, *bp;
    ap = a;
    bp = b;
    srand (time(NULL));   // set the random seed to current time	
    myrand(ap,bp,L,N,r);  //populate the arrays with random numbers

    // write random arrays to csv file
    int tmp;
    char mydata[500];   
    ofstream ofs;
    string mfn = "myrand.csv";    
    string colnames = "a,b\n";

    ofs.open(mfn.c_str(), ofstream::out | ofstream::app);
    ofs << colnames;

    for( int i = 0; i < N; i++){
      tmp = sprintf(mydata, "%f,%f\n",a[i],b[i]);  //write data to buffer: mydata
      ofs << mydata;    //write mydata to file mfn
    }

    ofs.close();

    return 0;
  }
}
