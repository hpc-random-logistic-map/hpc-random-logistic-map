#! /bin/bash
#SBATCH -N 1 
#SBATCH --time=0:09:00
#sBATCH --qos=janus


./generate_rands.exe -L 0.6 -r 3.2 -f myrand.csv

./myfunc.exe -L 0.6 -r 3.2 -x0 0.4 -iter 1000 -f myrand.csv 
#echo "                                            " >> Results.csv
