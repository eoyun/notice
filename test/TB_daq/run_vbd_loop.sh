#!/bin/bash

for var in {1..13}
do
  echo ./run_all.sh vbd/run$var 100000
  cat ./runnum.txt
  ./run_all.sh vbd/run$var 100000 > ./log_for_vbd/run$var.log
  sleep 10
done

