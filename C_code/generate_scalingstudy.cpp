/*
  code to generate command line input for the scaling study
  input: 
  - parameter: L, correlation length
  - parameter: dr, spacing between r values on [0,4]
  - parameter: dx, spacing between initial x0 values on [0,1]
  - parameter: iter, max number of iterates in fixed point iteration (cobweb)
  - -f filename result file that the procs report to
  output:
  - serial.sh, the shell file for a serial job
  - node_i.sh, where i = {1,2,4,8,10,12,16}. shell files that invoke i nodes and the load balancer
  - cmdline_i, where the number of lines is M = (1-2dx)/dx
  - submitjob.sh, where the number of lines is 7 (one for the serial job, 6 for the load balancer jobs)
  REMARK:
  - be sure to chmod 777 submitjob.sh for slurm.
  - the .out files contain the run times in seconds elapsed.
  INSTRUCTIONS:
  1. do a make clean to remove all the other .sh, .out, and cmdlines files
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
  int maxarg = 11;
  if(argc != maxarg){
    printf("Used %d arguments. Expected %d arguments. Try something like:\n ./generate_scalingstudy.exe -L 0.1 -dr 0.1 -dx 0.1 -iter 1000 -f result.csv",argc,maxarg);
  }
  else{
    double L, r, x, dr, dx;
    int tmp, numx, numr, iter, nc, nm, nd;
    char mydata[1000], mybuf[1000];   
    ofstream ofs, ofsdriver;
    char cmdlines[50], myrand[50], driver[50];
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

    //serial stuff
    string serial = "serial.sh";
    char myrandserial[1000];
    string header = "#!/bin/bash\n#SBATCH -N 1\n#SBATCH --ntasks-per-node=12\n#SBATCH --time=10:00:00\n#SBATCH --job-name=serial\n#SBATCH --output=serial.out\nSTART=$(date +%s)\n";

    ofsdriver.open(serial.c_str(), ofstream::out | ofstream::trunc);
    ofsdriver << header;  

    for(int j = 0; j < numr; j++){
      x = 0.0;
      r = r + dr;
      nm = sprintf(myrand, "myrand_serial_%d.csv",j);
      tmp = sprintf(myrandserial,"./generate_rands.exe -L %f -r %f -f %s\n",L,r,myrand);
      ofsdriver << myrandserial;

      for( int i = 0; i < numx; i++){
	x = x + dx;
	tmp = sprintf(mydata, "./myfunc.exe -L %f -r %f -x0 %f -iter %d -f %s >> %s\n",L,r,x,iter,myrand,resfile.c_str());  //write data to buffer: mydata
	ofsdriver << mydata;    //write mydata to file 
      }

    }
    header = "END=$(date +%s)\necho 0,$START,$END >> timings.csv\n";
    ofsdriver << header;
    ofsdriver.close();    

    //loadbalancer stuff
    int nodes[7] = {1,2,4,8,10,12,16};
    int numnodes = 7;

    for(int k = 0; k < numnodes; k++){   //loop in nodes
      r = 0.0;
      nd = sprintf(driver, "driver_%d.sh",nodes[k]);	

      //generate shell scripts driver_j.sh      
      ofsdriver.open(driver, ofstream::out | ofstream::trunc);
      tmp = sprintf(mybuf,"#!/bin/bash\n#SBATCH -N %d\n#SBATCH --ntasks-per-node=12\n#SBATCH --time=05:00:00\n#SBATCH --job-name=driver_%d\n#SBATCH --output=driver_%d.out\nmodule load loadbalance/load_balance_ib\n",nodes[k],nodes[k],nodes[k]);
      ofsdriver << mybuf; 
      header = "START=$(date +%s)\n";
      ofsdriver << header;

      for(int j = 0; j < numr; j++){   //loop in r
	x = 0.0;
	r = r + dr;
	nc = sprintf(cmdlines,"cmdline_%d_%f",nodes[k],r);
	nm = sprintf(myrand, "myrand_%d_%f.csv",nodes[k],r);

	tmp = sprintf(mybuf,"./generate_rands.exe -L %f -r %f -f %s\ntime mpirun load_balance -f %s -s dynamic\n",L,r,myrand,cmdlines);
	ofsdriver << mybuf;

	//generate cmdline_node[k]_r
	ofs.open(cmdlines, ofstream::out | ofstream::trunc);
	for( int i = 0; i < numx; i++){   //loop in x
	  x = x + dx;
	  tmp = sprintf(mydata, "./myfunc.exe -L %f -r %f -x0 %f -iter %d -f %s >> %s\n",L,r,x,iter,myrand,resfile.c_str());  //write data to buffer: mydata
	  ofs << mydata;    //write mydata to file 
	}
	ofs.close();        
      }      
      header = "END=$(date +%s)\n";
      ofsdriver << header;
      tmp = sprintf(mybuf,"echo %d,$START,$END >> timings.csv\n",nodes[k]);
      ofsdriver << mybuf;
      ofsdriver.close();
    }


    //generate submitjob.sh
    sjob = "submitjob.sh";
    ofs.open(sjob.c_str(), ofstream::out | ofstream::trunc);
    ofs << "chmod 777 driver_*\nchmod 777 serial.sh\n";
    ofs << "sbatch --qos=janus serial.sh\n";
    for(int j = 0; j < numnodes; j++){
      tmp = sprintf(mydata, "sbatch --qos=janus driver_%d.sh\n", nodes[j]);
      ofs << mydata;
    }
    ofs.close();

    return 0;

  }
}