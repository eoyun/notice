#!/bin/bash

source /Users/drc_daq/scratch/notice/notice_env.sh

runnum=`cat /Users/drc_daq/scratch/notice/test/TB_daq/runnum.txt`
echo "$runnum" is run number
./set_test.exe
echo Can u see me?