#!/bin/bash

source /Users/yhep/scratch/notice/notice_env.sh

FILE=/Users/yhep/scratch/notice/test/TB_daq/FAILSET

runnum=`cat runnum.txt`
echo run number is "$runnum"!!

runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt
read -p "Enter the set up config file : " setup
echo $setup
./src/set_run_number_CAL.exe $runnum

./src/set_bymid_mktxt_CAL.exe $setup >> ./log/log_set_$runnum.log 
if [ -f $FILE ]
then 
  echo check the config file name!!
  rm /Users/yhep/scratch/notice/test/TB_daq/FAILSET
else
  echo setting complete!
  read -p "Enter the nevt : " nevt
  
  midlist=`cat turn_on_mid.txt`
  echo mid list :
  echo "$midlist"
  
  num=1
  
  for var in $midlist
  
  do
     #echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
     ./src/get_evt_dir_log_CAL.exe $var $nevt >> ./log/log_"$var"_"$runnum".log &
     sleep 1
     echo "processing $num : mid num is $var"
     num=$((num+1))
  done
  echo ready!!
  
  sleep 1
  ./src/run_evt_mktxt_CAL.exe 
fi

