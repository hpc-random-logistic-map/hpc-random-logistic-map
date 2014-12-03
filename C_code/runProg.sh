#! /bin/bash
#SBATCH -N 1 
#SBATCH --time=0:09:00
#sBATCH --qos=janus


./generate_rands.exe -L 0.35 -r 2.8 -f myrand.csv

./myfunc.exe -L 0.35 -r 2.8 -x0 0.5 -iter 1000 -f myrand.csv
