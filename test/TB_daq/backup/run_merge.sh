#!/bin/bash

source /Users/drc_daq/scratch/notice/notice_env.sh

runnum=`cat runnum.txt`
echo run number is "$runnum"!!
runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt

./set_run_number_CAL.exe $runnum

./set_CAL.exe >> log_set_$runnum.log

echo setting complete!
read -p "if beam start, press Enter!! " run

echo argu : $@

./get_total_CAL.exe $@ >> log_total_$runnum.log &

echo ready!!

read -p "Enter the run time : " time
./run_CAL.exe $time


