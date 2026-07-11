#!/bin/zsh

mid=$1
runnum=$2

	while true :
	do

		var=$(ps -ef | grep ./run_all.sh | awk /bash/'{print $2}')
		pid=$(echo ${var})

		#echo ${pid}

		tail -n 2 log/log_${mid}_${runnum}.log

		sleep 1

		if [ -z "${pid}" ]
		then
			break
		fi

	done 
