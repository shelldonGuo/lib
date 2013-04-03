#!/bin/sh


HADOOP_CLIENT_BIN="./hadoop/bin/hadoop"

SET_A_INPUT_DATA="/user/data/inputA"
SET_B_INPUT_DATA="/user/data/inputB"
OUTPUT_DIR="/user/data/output"


# get inter_set of two set: a&b
function inter_set(){
	$HADOOP_CLIENT_BIN dfs -rmr $OUTPUT_DIR

	$HADOOP_CLIENT_BIN streaming \
	-D mapred.job.priority='HIGH' \
	-D mapred.map.tasks=2000 \
	-D mapred.reduce.tasks=500 \
	-D stream.memory.limit=4000 \
	-D mapred.job.map.capacity=1000 \
	-D mapred.map.over.capacity.allowed=false \
	-D mapred.reduce.over.capacity.allowed=false \
	-D mapred.map.capacity.per.tasktracker=2 \
	-D mapred.job.name=inter_set \
	-mapper "sh mapper.sh" \
	-reducer "python reducer_inter_set.py" \
	-input ${SET_A_INPUT_DATA} \
	-input ${SET_B_INPUT_DATA} \
	-output ${OUTPUT_DIR} \
	-file reducer_inter_set.py \
	-file mapper.sh

	if [ $? -ne 0 ];then
		echo "hadoop job failed"
		exit 1
	fi

	$HADOOP_CLIENT_BIN dfs -get $OUTPUT_DIR ./output
	if [ $? -ne 0 ];then
		echo "get output to local failed"
		exit 1
	fi

	echo "job finish"
}

inter_set

exit 0
