#!/bin/bash

time0=$(date +%s)
for i in {1..15}
do
echo ${i}
rsync -avh --progress /Volumes/HDD_16TB_1/HDD_speed_test/wave/mid_${i} sungwon@lxplus.cern.ch:/eos/user/s/sungwon/rsync | tee -a rsync_daq16tb2cbox_swkim_mid${i}.log
done
time1=$(date +%s)
echo "duration"
echo "$(($time1-$time0))"
