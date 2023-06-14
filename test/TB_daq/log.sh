#!/bin/bash

# after doing the command [tail -f log/log_10_3202.log]

# var1=$(ps -ef|grep 3202|grep -v 'grep'|grep -v $0)
# pid1=$(echo ${var1} | cut -d " " -f2)

# if [ -n "${pid1}" ]
# then 
# 	tail log/log_10_3201.log
# 	var2=$(ps -ef|grep 3201|grep -v 'grep'|grep -v $0)
# 	pid2=$(echo ${var2} | cut -d " " -f2)
# else
# 	kill -9 ${pid2}
# fi

while [ 1 ]
do 
	pid1=`ps -ef | grep 3202 | grep -v 'grep' | awk '{print $2}'`
	if [ -z $pid1 ]
	then		
		tail log/log_10_3201.log
		var2=$(ps -ef|grep 3201|grep -v 'grep'|grep -v $0)
		pid2=$(echo ${var2} | cut -d " " -f2)
	else
		kill -9 ${pid2}
	fi
done




