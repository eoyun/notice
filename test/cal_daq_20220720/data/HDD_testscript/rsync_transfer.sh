#!/bin/bash

time0=$(date +%s)
for i in {1..15}
do
echo ${i}
rsync -vrz /Volumes/HDD_16TB_1/HDD_speed_test/fast/mid_${i} yson@lxplus.cern.ch:/eos/user/y/yson/drctb/rsync 2> rsync_daq16tb2cbox_mid${i}.log
done
time1=$(date +%s)
echo "duration"
echo "$(($time1-$time0))"
