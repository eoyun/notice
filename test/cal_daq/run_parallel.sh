#!/bin/bash

runnum=`cat runnum.txt`
echo run number is "$runnum"!!

runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt

stdbuf -oL ./set_run_number_CAL.exe $runnum

stdbuf -oL ./set_CAL.exe >> log_set_$runnum.log 

echo setting complete!
read -p "if beam start, press Enter!!" run

for var in $@

do
	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
	stdbuf -oL ./run_daq_nodiv_CAL.exe $var >> log_$var.$runnum.log &
	sleep 1
	echo processing $var / $#
done
echo ready!!

read -p "Enter the run time : " time
stdbuf -oL ./run_tcb_CAL.exe $time


