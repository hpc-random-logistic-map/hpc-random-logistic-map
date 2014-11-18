// my first program in C++
//Amy Le
//October 9 2014

#include <iostream> //cout
#include <fstream> //write to csv; ofstream
#include <string> //string
#include <cstring>
#include <stdlib.h> //atof, atoi, itoa
#include <sys/time.h>  //walltime
#include <math.h> //pow
#include <omp.h>
#include <hdf5.h> 
using namespace std;

double get_wall_time(){
  struct timeval time;
  if (gettimeofday(&time,NULL)){
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec/1000000.0;
}

int main(int argc, char* argv[])
{
  int maxarg = 4;
  if ( argc != maxarg ){ // argc should be 4 for correct execution
    cout<<"Too few arguments. Please enter 3 file names.";
  }
  else {
    ofstream ofs;
    int acol, arow, bcol, brow, ccol, crow, i, j, k;
    int nt;
    string cs; 
    
    //read the HDF5 file
    string mat_a = argv[1];
    string mat_b = argv[2];

    herr_t  status;
    hid_t file_a = H5Fopen (mat_a.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t file_b = H5Fopen (mat_b.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    hid_t dset_a = H5Dopen (file_a, "x", H5P_DEFAULT);
    hid_t dset_b = H5Dopen (file_b, "x", H5P_DEFAULT);
    hid_t space_a = H5Dget_space (dset_a);
    hid_t space_b = H5Dget_space (dset_b);
    hsize_t dims_a[2], mdims_a[2];
    hsize_t dims_b[2], mdims_b[2];
    status = H5Sget_simple_extent_dims(space_a,dims_a,mdims_a);
    status = H5Sget_simple_extent_dims(space_b,dims_b,mdims_b);

    arow = dims_a[0];
    acol = dims_a[1];
    brow = dims_b[0];
    bcol = dims_b[1];
    crow = arow;
    ccol = bcol;      

    //initialize the pointers
    double *a, *b, *c;
    a = new double[arow*acol];
    b = new double[brow*bcol];
    c = new double[crow*ccol];

#pragma omp parallel private(i) shared(a,b,c)
    {
      nt = omp_get_num_threads();
#pragma omp for 
      for (i = 0; i < (arow*acol); i++) {
	a[i] = 0.0;	  
      }
#pragma omp for       
      for (i = 0; i < (brow*bcol); i++) {
	b[i] = 0.0;
      }
#pragma omp for       
      for (i = 0; i < (crow*ccol); i++) {
	c[i] = 0.0;
      }
    }
  		
    hid_t dcpl_a = H5Dget_create_plist (dset_a);
    hid_t dcpl_b = H5Dget_create_plist (dset_b);
    H5D_layout_t layout_a = H5Pget_layout (dcpl_a);
    H5D_layout_t layout_b = H5Pget_layout (dcpl_b);

    status = H5Dread (dset_a, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, a);
    status = H5Dread (dset_b, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, b);
   
    status = H5Pclose (dcpl_a);
    status = H5Pclose (dcpl_b);
    status = H5Dclose (dset_a);
    status = H5Dclose (dset_b);
    status = H5Fclose (file_a);	
    status = H5Fclose (file_b);	
      
    /*    cout << "start matrix a:\n";
    for (i = 0; i < arow*acol; i++){
      if (i%acol == 0){
	cout << "\n";
      }
      cout << a[i] << " ";
    }
    cout << "\nend of matrix a\n";

    cout << "\nstart matrix b:\n";
    for (i = 0; i < brow*bcol; i++){
      if (i%bcol == 0){
	cout << "\n";
      }
      cout << b[i] << " ";
    }
    cout << "\nend of matrix b\n";*/

    /*    string fname = "mat5.csv";
    ofs.open (fname.c_str(), ofstream::out | ofstream::app);
    ofs << arow << " " << acol << "\n";
    for (i = 0; i < arow*acol; i++){
      if(i%acol == 0){
	ofs << "\n";
      }
      ofs << a[i] << " ";
    }    
    ofs.close();*/



    double mstart = get_wall_time();      
    double mysum;
#pragma omp parallel private(i,j,k) shared(c)
    {
#pragma omp for reduction(+:mysum) 
    for( int i = 0; i < crow; ++i){
      for (int j = 0; j < ccol; ++j){
	mysum = 0;
	for (int k = 0; k < acol; k++){
	  mysum += a[i*acol + k] * b[k*bcol + j];
	}
	c[i*ccol + j] = mysum;
      }
    }
    }      
    double mend = get_wall_time();
    double ptime = (mend - mstart);
    double numflops = 2;
    double numerator = numflops*(double)arow*(double)bcol*(double)acol;
    double denom = pow(10,6)*ptime;
    double mflops = numerator/denom;
    double ti = ptime/(1.0);

    /*    cout << "\nstart matrix c:\n";
    for (i = 0; i < crow*ccol; i++){
      if (i%ccol == 0){
	cout << "\n";
      }
      cout << c[i] << " ";
    }
    cout << "\nend of matrix c.\n";*/


    // write the hdf5 data    UNCOMMENT THIS FOR TURNING IN 
    hid_t fid, did, sid, tid, gid;
    herr_t ierr;
    hsize_t dims[2] = {crow, ccol};
    string filename = argv[3]; // destination file name is argv[3]
    const char grp[] = "dataset";
      
    fid = H5Fcreate (filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);  //create file.
    gid = H5Gcreate2(fid, grp,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    sid = H5Screate_simple (2, dims, NULL); //dataspace. rank is 2 for the 2-d matrix
    tid = H5Tcopy(H5T_IEEE_F64LE);		//set up the data type
    did = H5Dcreate (gid, "c", tid, sid, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT); // dataset
    ierr = H5Dwrite (did, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, c);  //write c

    ierr = H5Dclose(did);
    ierr = H5Tclose(tid);
    ierr = H5Sclose(sid);
    ierr = H5Gclose(gid);
    ierr = H5Fclose(fid);

    //reset c
#pragma omp parallel for private(i) shared(c)
    for(i = 0; i < crow*ccol; i++){
      c[i] = 0.0;
    }

    //serial code
    double t0 = get_wall_time();
    for( int i = 0; i < crow; ++i){
      for (int j = 0; j < ccol; ++j){
	mysum = 0;
	for (int k = 0; k < acol; k++){
	  mysum += a[i*acol + k] * b[k*bcol + j];
	}
	c[i*ccol + j] = mysum;
      }
    }      
    double tf = get_wall_time();
    double stime = tf-t0;

    double su = stime/ptime;
    double eff = su/nt;
    double sf = ( (1/su)-(1/nt) ) / ( 1 - (1/nt) );

    //write the results 
    int tmp1, tmp2;
    char mfn[50], mydata[500];
    tmp1 = sprintf(mfn, "matmul%dx%d.csv",crow,ccol);   //get file name for writing
    //    string colnames = "nthreads,time/iteration,MFLOP/s,Speedup,Efficiency,Serial-Fraction\n";
    tmp2 = sprintf(mydata, "%d,%f,%f,%f,%f,%f\n",nt,ti,mflops,su,eff,sf);   //write data to buffer: mydata
    //    cout << mydata;
    ofstream ofs2;
    ofs2.open(mfn, ofstream::out | ofstream::app);
    //ofs << colnames;
    ofs2 << mydata;    //write mydata to file mfn
    ofs2.close();

    //clean up and exit
    delete [] a;    
    delete [] b;    
    delete [] c;    
  }
  return 0;
}
						
