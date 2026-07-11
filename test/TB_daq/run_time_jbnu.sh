#!/bin/bash

source /Users/yhep/scratch/notice/notice_env.sh

FILE=/Users/yhep/scratch/notice/test/TB_daq/FAILSET
RUNFILE=/Users/yhep/scratch/notice/test/TB_daq/RUNNUMFAIL

runnum=`cat runnum.txt`
runnum1=`cat ./backup/runnum/runnum.txt`
if [ $runnum != $runnum1 ]; then
  echo check the run number
  exit
fi

echo run number is "$runnum"!!
read -p "Enter the set up config file : " setup
echo $setup
runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt
echo "$runnumtemp" > ./backup/runnum/runnum.txt

./src/set_run_number_CAL.exe $runnum


#./set_CAL.exe >> ./log/log_set_$runnum.log 
./src/set_bymid_CAL.exe $setup >> ./log/log_set_$runnum.log 
./src/set_JBNU_DAQ_anc.exe $setup >> ./log/log_set_anc_$runnum.log 

if [ -f $FILE ]
then  
  echo check the config file name!!
  rm /Users/yhep/scratch/notice/test/TB_daq/FAILSET
else
  echo setting complete!
  read -p "Enter the time : " time
  
  num=1
  
  for var in $@
  
  do
  	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
  	./src/daq_JBNU_DAQ.exe $var >> ./log/log_"$var"_"$runnum".log &
  	sleep 1
  	echo "processing $num / $# : mid num is $var"
  	num=$((num+1))
  done
  echo ready!!
  
  sleep 1
  ./src/run_CAL.exe $time
fi

