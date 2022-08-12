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

    for mid in 1 2
    do 
       root -b -l -q  plot_waveform_32ch.C'("./data/cal_wave_'$mid'_'$runnum'.dat", '$rmin', '$rmax' )';
    done
    open ./data/cal_wave_*_$runnum.dat*.png

	else
    cd ..
    echo "$FILE does not exist!!!! Please check run number again!!!!!"
fi

