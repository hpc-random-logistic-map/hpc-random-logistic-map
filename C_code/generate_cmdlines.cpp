/*
  code to generate command line input for the scaling study
  input: 
  - parameter: L, correlation length
  - parameter: dr, spacing between r values on [0,4]
  - parameter: dx, spacing between initial x0 values on [0,1]
  - parameter: iter, max number of iterates in fixed point iteration (cobweb)
  - -f filename result file that the procs report to
  output:
  - M driver_i.sh files, where i = 0...M-1 and M = (4-2dr)/dr
  - M cmdline_i files, where the number of commands is N = (1-2dx)/dx
  - 1 submitjob.sh file, where there are M lines, and each corresponds with driver_i.sh file
  REMARK:
  - be sure to chmod 777 submitjob.sh for slurm.
  - do something like cat res* > output_MM.DD.YY.csv to combine all files at the end of the run
  INSTRUCTIONS:
  1. do a make clean to remove all the other .sh, .out, res*, myrand*, and cmdlines files
  2. load all module files (load balancer, slurm, icpc, etc.) and make
  3. run something like ./generate_cmdlines.exe -L 0.1 -dr 0.1 -dx 0.1 -iter 1000 -f result.csv
  4. chmod 777 submitjob.sh
  5. on an interactive session, run ./submitjob.sh
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
  int maxarg = 9;
  if(argc != maxarg){
    printf("Used %d arguments. Expected %d arguments. Try something like:\n ./generate_cmdlines.exe -L 0.1 -dr 0.1 -dx 0.1 -iter 1000",argc,maxarg);
  }
  else{
    double L, r, x, dr, dx;
    int tmp, numx, numr, iter, nc, nm, nd, nr;
    char mydata[1000], mybuf[1000];   
    ofstream ofs, ofsdriver;
    char cmdlines[50], myrand[50], driver[50], result[50];
    string sjob, resfile;
    string c;

    for (int i = 1; i < maxarg; i+=2){
      c = argv[i];
      if(c == "-L"){
	L = atof(argv[i+1]);   // sim param L in [0,1]
      }
      if(c == "-dr"){
	dr = atof(argv[i+1]);    
      }
      if(c == "-iter"){
	iter = atoi(argv[i+1]);
      }
      if(c == "-dx"){
	dx = atof(argv[i+1]);
      }
      if(c == "-f"){
	resfile = argv[i+1];
      }
    }

    numr = ((4-dr) - (0+dr))/dr;
    numx = ((1-dx) - (0+dx))/dx;
    r = 0.0;

    for(int j = 0; j < numr; j++){
      x = 0.0;
      r = r + dr;
      nc = sprintf(cmdlines,"cmdline_%d",j);
      nm = sprintf(myrand, "myrand_%d.csv",j);
      nd = sprintf(driver, "driver_%d.sh",j);
      nr = sprintf(result, "result_%d.csv",j);

      //generate shell scripts driver_j.sh      
      ofsdriver.open(driver, ofstream::out | ofstream::trunc);
      tmp = sprintf(mybuf,"#!/bin/bash\n#SBATCH -N 1\n#SBATCH --ntasks-per-node=12\n#SBATCH --time=01:00:00\n#SBATCH --job-name=driver_%d\n#SBATCH --output=driver_%d.out\nmodule load loadbalance/load_balance_ib\n./generate_rands.exe -L %f -r %f -f myrand_%d.csv\ntime mpirun load_balance -f cmdline_%d -s dynamic\n",j,j,L,r,j,j);
      ofsdriver << mybuf; 
      ofsdriver.close();

      //generate cmdline_j
      ofs.open(cmdlines, ofstream::out | ofstream::trunc);
      for( int i = 0; i < numx; i++){
	x = x + dx;
	tmp = sprintf(mydata, "./myfunc.exe -L %f -r %f -x0 %f -iter %d -f %s >> %s\n",L,r,x,iter,myrand,result);  //write data to buffer: mydata
	ofs << mydata;    //write mydata to file 
      }
      ofs.close();
    }    

    //generate submitjob.sh
    sjob = "submitjob.sh";
    ofs.open(sjob.c_str(), ofstream::out | ofstream::trunc);
    ofs << "chmod 777 driver_*\n";
    for(int j = 0; j < numr; j++){
      tmp = sprintf(mydata, "sbatch --qos=janus driver_%d.sh\n", j);
      ofs << mydata;
    }
    ofs.close();

    return 0;

  }
}
