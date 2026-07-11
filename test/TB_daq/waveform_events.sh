#!/bin/zsh

startrun=$1
endrun=$2

for (( i = $1; i < $2 + 1; i++ ))
  do
    
    tail -n 1 ./log/log_10_$i.log | grep Waveform > /dev/null 2>&1

    if [ $? -eq 0 ]
    then echo [ run $i ] waveform : `tail -n 1 ./log/log_10_$i.log | grep -o '[0-9]*'` events
    else echo [ run $i ] waveform : `tail -n 2 ./log/log_10_$i.log | grep Waveform | grep -o '[0-9]*'` events
    fi

  done
