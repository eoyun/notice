#!/bin/bash

for var in {1..15}

do
	#echo stdbuf -oL ./run_daq_nodiv_CAL.exe $var 
	stdbuf -oL ./run_daq_nodiv_CAL.exe $var >> log_1_$var.log &
	sleep 1
done
echo ready!!
