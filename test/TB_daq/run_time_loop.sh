#!/bin/bash

source /Users/yhep/scratch/notice/notice_env.sh

FILE=/Users/yhep/scratch/notice/test/TB_daq/FAILSET
RUNFILE=/Users/yhep/scratch/notice/test/TB_daq/RUNNUMFAIL
FLAGFILE=/Users/yhep/scratch/notice/test/TB_daq/FLAGKILL

time=$2
loop=$3
setup='setup'

for ((i = 0; i < $loop ; i++))
do
  if [ -f $FLAGFILE ]
  then 
    echo kill all run
    rm $FLAGFILE
  else
  runnum=`cat runnum.txt`
  runnum1=`cat ./backup/runnum/runnum.txt`
  if [ $runnum != $runnum1 ]; then
    echo check the run number
    exit
  fi

  echo run number is "$runnum"!!
  echo config file name : setup
# read -p "Enter the set up config file : " setup
# echo $setup
  runnumtemp=$((runnum+1))
  echo "$runnumtemp" > runnum.txt
  echo "$runnumtemp" > ./backup/runnum/runnum.txt

  ./src/set_run_number_CAL.exe $runnum


#./set_CAL.exe >> ./log/log_set_$runnum.log 
  ./src/set_bymid_CAL.exe $setup >> ./log/log_set_$runnum.log 

# if [ -f $FILE ]
# then  
  # echo check the config file name!!
  # rm /Users/yhep/scratch/notice/test/TB_daq/FAILSET
# else
  echo setting complete!
  # read -p "Enter the time : " time
  
  num=1
  
  for var in $1
  
  do
  	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
  	./src/get_CAL.exe $var >> ./log/log_"$var"_"$runnum".log &
  	sleep 1
  	echo "processing $num / $# : mid num is $var"
  	num=$((num+1))
  done
  echo ready!!
  
  sleep 1
  ./src/run_CAL.exe $time
# fi

  sleep 3
fi
done
if [ -f $FLAGFILE ]
then 
  echo kill all run
  rm $FLAGFILE
fi

