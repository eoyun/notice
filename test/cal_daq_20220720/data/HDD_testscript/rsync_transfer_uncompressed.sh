#!/bin/bash

time0=$(date +%s)
/Volumes/HDD_16TB_1/HDD_speed_test/fast/mid_1
rsync -vr /Volumes/HDD_16TB_1/HDD_speed_test/fast/mid_1 yson@lxplus.cern.ch:/eos/user/y/yson/drctb/rsync 2> rsync_daq16tb2cbox_mid1_uncompressed.log
time1=$(date +%s)
echo "duration"
echo "$(($time1-$time0))"
