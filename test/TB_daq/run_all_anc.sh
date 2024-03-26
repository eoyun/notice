#!/bin/bash
# ./run_all.sh config nevt
source /Users/yhep/scratch/notice/notice_env.sh

setup=$1
nevt=$2

FILE=/Users/yhep/scratch/notice/test/TB_daq/FAILSET
RUNFILE=/Users/yhep/scratch/notice/test/TB_daq/RUNNUMFAIL
SETFILE=/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid.txt
SETANCFILE=/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid_anc.txt

runnum=`cat runnum.txt`
runnum1=`cat ./backup/runnum/runnum.txt`
if [ $runnum != $runnum1 ]; then
  echo check the run number
  exit
fi

echo run number is "$runnum"!!

runnumtemp=$((runnum+1))
echo "$runnumtemp" > runnum.txt
echo "$runnumtemp" > ./backup/runnum/runnum.txt
echo $setup
./src/set_run_number_CAL.exe $runnum

./src/set_bymid_mktxt_CAL.exe $setup >> ./log/log_set_$runnum.log
./src/set_JBNU_DAQ_anc_all.exe $setup >> ./log/log_set_anc_$runnum.log 
if [ -f $SETFILE ] || [ -f $SETANCFILE ]
then
  if [ -f $FILE ]
  then 
    echo check the config file name!!
    rm /Users/yhep/scratch/notice/test/TB_daq/FAILSET
  else
    echo setting complete!
  
    midlist=`cat turn_on_mid.txt`
    midanclist=`cat turn_on_mid_anc.txt`
    echo mid list :
    echo "$midlist"
    echo "$midanclist"
  
    num=1
    
    for var in $midlist
    
    do
       #echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
       ./src/get_evt_dir_log_CAL.exe $var $nevt >> ./log/log_"$var"_"$runnum".log &
       sleep 1
       echo "processing $num : mid num is $var"
       num=$((num+1))
    done
    for varanc in $midanclist
    do
  	   ./src/daq_JBNU_DAQ.exe $varanc >> ./log/log_"$varanc"_"$runnum".log &
       sleep 1
       echo "processing $num : anc mid num is $varanc"
       num=$((num+1))
    done
    rm $SETANCFILE

    echo ready!!
    
    if [ -f $RUNFILE ]; then
      echo TCB run number error!!
      touch /Users/yhep/scratch/notice/test/TB_daq/KILLME
      rm /Users/yhep/scratch/notice/test/TB_daq/RUNNUMFAIL
    fi
    sleep 1
    ./src/run_evt_mktxt_CAL.exe
  fi
else
  echo setting bug plz rerun
fi

