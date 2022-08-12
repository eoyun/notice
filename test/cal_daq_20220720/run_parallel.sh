#!/bin/bash

source /Users/drc_daq/scratch/notice/notice_env.sh

runnum=`cat runnum.txt`
echo run number is "$runnum"!!

runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt

./set_run_number_CAL.exe $runnum

./set_CAL.exe >> ./log/log_set_$runnum.log 

echo setting complete!
read -p "if beam start, press Enter!!" run

num=1

for var in $@

do
	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
	./get_CAL.exe $var >> ./log/log_"$var"_"$runnum".log &
	sleep 0.2
	echo "processing $num / $# : mid num is $var"
	num=$((num+1))
done
echo ready!!

read -p "Enter the run time : " time
./run_CAL.exe $time


