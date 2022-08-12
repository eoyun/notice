#!/bin/bash

# source  draw.sh [RunNumber] [rmin,3000] [rmax, 4096] [Nevt, 10]
#										            (0~4096 ADC), Nevt (1evt = 1s)
echo $1
echo $2
fname=$1
rmin_=$2
rmax_=$3
a=$4
rmin=$(($rmin_ * $a))
rmax=$(($rmax_ * $a))

FILE=cal_wave_1_$fname.dat
echo $FILE
cd data

if [ -f "$FILE" ]; then
    echo "$FILE exists."
    cd ..

    for mid in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    do
       #root -b -q -l plot_spectrum_32ch_update.C\(\"${fname}\"\); 
       #echo "root -b -q -l plot_waveform_32ch_AvgTimeStr.C\(\"${fname}\", ${rmin_}, ${rmax_} , \"${condi}\"\); "
       root -l -q -b plot_spectrum_32ch_SaveAll.C'("./data/cal_wave_'$mid'_'$fname'.dat", '$rmin_', '$rmax_' )'; 
       root -l -q -b plot_waveform_32ch_AvgTimeStr_SaveAll.C'("./data/cal_wave_'$mid'_'$fname'.dat", '$rmin_', '$rmax_' )'; 
       root -l -q -b plot_waveform_32ch_SaveAll.C'("./data/cal_wave_'$mid'_'$fname'.dat", '$rmin_', '$rmax_' )'; 
    done
    
#    echo "open ./data/cal_wave_*_$fname.dat*.png"
#    open ./data/cal_wave_*_$fname.dat*.png

	else
    cd ..
    echo "$FILE does not exist!!!! Please check run number again!!!!!"
fi

##root -b -q -l plot_spectrum_32ch_update.C'("./data/cal_wave_8_130.dat","CableOnly")'; 
##root -b -q -l plot_waveform_32ch_AvgTimeStr.C'("./data/cal_wave_8_130.dat",2000*1000, 4096*1000,"CableOnly")'; 
##root -l plot_waveform_32ch.C'("./data/cal_wave_8_130.dat",2000,4096,"CableOnly")'; 
