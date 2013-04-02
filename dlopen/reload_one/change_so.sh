#!/bin/bash
flag=$1
if [ $flag == "add" ];then
	rm libsthc.so
	cp libadd.so libsthc.so
elif [ $flag == "min" ];then
	rm libsthc.so
	cp libmin.so libsthc.so
fi
