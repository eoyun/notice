#!/bin/bash

midlist=`cat test.txt`

echo "${#midlist[0]}"
for var in $midlist

do
  echo test : $var 
done 
