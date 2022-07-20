#!/bin/bash

runnum=`cat runnum.txt`
echo run number is "$runnum"!!
runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt

stdbuf -oL ./set_run_number_CAL.exe $runnum

stdbuf -oL ./set_CAL.exe >> log_set_$runnum.log

echo setting complete!
read -p "if beam start, press Enter!! " run

echo argu : $@

stdbuf -oL ./run_daq_total_CAL.exe $@ >> log_total_$runnum.log &

echo ready!!

read -p "Enter the run time : " time
stdbuf -oL ./run_tcb_CAL.exe $time

runnum=$((runnum+1))
echo "$runnum" > runnum.txt

