#!/bin/zsh

source /Users/yhep/scratch/notice/notice_env.sh

file=/Users/yhep/scratch/notice/test/TB_daq/runnum.txt
echo ready!!

if test -s $file ; then
  echo file exist
  exit
fi
echo hello


