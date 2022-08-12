#!/bin/bash

echo Data transfer : DAQ to external HDD 8 TB
time0=$(date +%s)
cp -r /Users/drc_daq/scratch/notice/test/cal_daq_20220720/data/HDD_speed_test /Volumes/HDD_8TB_1
time1=$(date +%s)
echo "duration"
echo "$(($time1-$time0))"
