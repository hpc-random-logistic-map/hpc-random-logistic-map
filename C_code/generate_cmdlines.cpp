/*
  code to generate command line input to the load balancer
  input:
  input: 
  - parameter: L, correlation length
  - parameter: r
  output:
  - file containing a list of commands for the load balancer
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

int main(int argc, char* argv[]){
    int maxarg = 13;
    if(argc != maxarg){
	cout << argc << " arguments. Incorrect inputs. Try something like:\n ./generate_cmdlines.exe -L 0.1 -r 3.2 -iter 1000 -h 0.01 -f cmdlines -ab myrand.csv";
    }
    else{
	double L, r, x, h;
	int tmp, N, iter;
	char mydata[500];   
	ofstream ofs;
	string mfn, ab;
	string c;

	for (int i = 1; i < maxarg; i+=2){
	    c = argv[i];
	    if(c == "-L"){
		L = atof(argv[i+1]);   // sim param L in [0,1]
	    }
	    if(c == "-r"){
		r = atof(argv[i+1]);    // sim param r in [0,4]
	    }
	    if(c == "-iter"){
		iter = atoi(argv[i+1]);
	    }
	    if(c == "-h"){
		h = atof(argv[i+1]);
	    }
	    if(c == "-f"){
		mfn = argv[i+1];
	    }
	    if(c == "-ab"){
	        ab = argv[i+1];
	    }
	}
	N = 1/h;
	x = 0.0;
	
	ofs.open(mfn.c_str(), ofstream::out | ofstream::trunc);
	for( int i = 0; i < N; i++){
	    x = x + h;
	    tmp = sprintf(mydata, "./myfunc.exe -L %f -r %f -x0 %f -iter %d -f %s \n",L,r,x,iter,ab.c_str());  //write data to buffer: mydata
	    ofs << mydata;    //write mydata to file mfn
	}

	ofs.close();

	return 0;

    }
}
