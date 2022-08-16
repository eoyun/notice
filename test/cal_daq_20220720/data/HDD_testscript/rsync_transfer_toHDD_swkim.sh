#!/bin/bash

time0=$(date +%s)
for i in {1..15}
do
echo ${i}
rsync -avh --progress /Users/drc_daq/scratch/notice/test/cal_daq_20220720/data/HDD_speed_test/wave/mid_${i} /Volumes/HDD_8TB/HDD_speed_test | tee -a rsync_SSDToHDD1_swkim_mid${i}.log
done
time1=$(date +%s)
echo "duration"
echo "$(($time1-$time0))"
