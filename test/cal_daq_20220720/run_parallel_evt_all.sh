#!/bin/bash

source /Users/drc_daq/scratch/notice/notice_env.sh

runnum=`cat runnum.txt`
echo run number is "$runnum"!!

runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt
read -p "Enter the set up config file : " setup
echo $setup
./set_run_number_CAL.exe $runnum

./set_bymid_mktxt_CAL.exe $setup >> ./log/log_set_$runnum.log 

echo setting complete!
read -p "Enter the nevt : " nevt

midlist=`cat turn_on_mid.txt`
echo "$midlist"

num=1

for var in $midlist

do
	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
	./get_evt_CAL.exe $var $nevt >> ./log/log_"$var"_"$runnum".log &
	sleep 0.2
	echo "processing $num / $# : mid num is $var"
	num=$((num+1))
done
echo ready!!

sleep 0.2
./run_evt_mktxt_CAL.exe 


