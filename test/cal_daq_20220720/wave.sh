#!/bin/bash

runnum=$1
rmin=$2
rmax=$3

FILE=cal_wave_1_$runnum.dat
echo $FILE
cd data

if [ -f "$FILE" ]; then
    echo "$FILE exists."
    cd ..
	#for mid in 1 2 3 4 5 5 6 7 8 9 10 11 12 13 14 15
    for mid in 1 2 
    do 
    root -b -l -q  plot_waveform_32ch.C'("./data/cal_wave_'$mid'_'$runnum'.dat", '$rmin', '$rmax' )';
    done
    
    open ./data/cal_wave_*_$runnum.dat*.png

	else
    cd ..
    echo "$FILE does not exist!!!! Please check run number again!!!!!"
fi
