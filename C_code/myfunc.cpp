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

/*
Period checking
 */
int period_check(int size, double* xv)
{
    double tol = pow(10,-6);
    int maxp = floor((size-100)/2);
    bool flag;
    //cout << maxp <<  " "<< size <<endl;
    for (int p=1; p <=maxp; p++)
    {
        flag = true;
        for (int i=size-1; i>size -1-p;i--)
        {
	  //            cout << xv[i] << " " << xv[i-p]<<endl;
            if(abs(xv[i] - xv[i-p])>tol)
            {
            //    cout << " correct " <<endl;
            flag = false;
            break;
            }
        }
        if (flag == true)
            return p;

    }
    return 0;
    //actual data from 101 to size-1 of xv
}

/*
calculate random function at location x
input: 
- x position 
- pointer to random array ab, where col1 = a, col2 = b
- simulation param r
- number of fourier modes N
*/
double R(double x, double **ab, double r, int N){
  double mysum = 0.0;
  double fs = 0.0;
    
  //reduction on mysum?
  for(int j = 0; j < N; j++){
    fs = 2 * ab[j][0] * cos(2*M_PI*j*x) - ab[j][1] * sin(2*M_PI*j*x);
    mysum = mysum + fs;
  }

  double xi = log(r) + mysum;
  double y = exp(xi);
  if (y > 4){
    cout << "Trouble! r > 4\n";
  }
  return y;
}

/*
  cobweb aka fixed point iteration
  - x0 is the initial condition
  - iter is the max number of iterates to carry out
  - *xv is the pointer to the result array (length = iter)
  - **ab is the pointer to the random values array col1 = a, col2 = b
  - r is the simulation parameter
  - N is the max number of Fourier modes
*/
void cobweb(double x0, int iter, double *xv, double **ab, double r, int N){
//  double *xold;
  int period = 0;
//  *xv = x0;
  // xold = xv;   // pointer to the previous iterate
  xv[0] = x0;
  for (int i = 0; i < iter-1; i++){
      xv[i+1] = R(xv[i], ab, r, N) * xv[i] * ( 1 - xv[i] );        
      cout<< xv[i]<<" ";
      if (i>101)   // possible optimization: simd vectorization remove index dependency
      {
        period = period_check(i,xv);
        if (period>0)
        {
            //sort and printing
            cout <<"period found"<< endl;
            cout << r <<" "<< 10.0/double(N) <<" "<< period <<endl;
            // for (int k = size-1 ; k>size -1 -p; k--)
            //     //sort_dataset(&xv[size-1-p]);
            //     print *xv;
            
            exit(0);
        }
       }
    
  }
}

int main(int argc, char* argv[]){
  int maxarg = 11;
  if(argc != maxarg){
    cout << argc << "Incorrect inputs. Try something like:\n ./myfunc.exe -L 0.1 -r 3.2 -x0 0.5 -iter 1000 -f myrand.csv";
  }
  else{
    double L, r, x0;
    string c, rfn;
    int iter;

    for (int i = 1; i < maxarg; i+=2){
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
      if(c == "-f"){
	rfn = argv[i+1];
      }
    }
    ifstream randfile (rfn.c_str());    	
    int N = 10/L;    //number of fourier modes    

    //read in the random a,b from file specified at cmdln
    string tmp;
    int tmpN;
    getline(randfile, tmp);   // read the file size
    tmpN = atoi(tmp.c_str());

    // make sure the file size matches N
    if( tmpN != N ){
      printf("Mismatch between number of random coefficients and correlation length. Number of random coefficients = %d. Number of Fourier Modes for correlation length L = %f is N = %d.\n",tmpN, L, N);
      return 0;
    } else{   
      
      // set up pointer to result array
      double xv[iter];

      // initialize the pointers to random array
      double **ab;
      ab = new double*[N];
      for (int i = 0; i < N; i++){
	ab[i] = new double[2];
      }

      //read in the random a,b from file specified at cmdln to ab
      for( int i = 0; i < N; i++){
	for( int j = 0; j < 2; j++){
	  randfile >> ab[i][j];
	}
      }
    //exit(0);
      // initialize the xv result array with 100 elements
      cobweb(x0, iter, xv, ab, r, N);

      // delete pointers
      for(int i = 0; i < N; i++){
	delete [] ab[i];
      }
      delete [] ab;
       return 0;
    }
  }
}
