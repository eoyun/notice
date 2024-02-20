#!/bin/bash

var=uh960307!@#$

#echo rsync --rsh=\"sshpass -p $var ssh -l eoyun\" cms.knu.ac.kr:~/ ./TEST
#rsync --rsh=\"sshpass -p $var ssh -l eoyun\" cms.knu.ac.kr:~/ ./TEST
sshpass -p $var ssh eoyun@cms.knu.ac.kr
