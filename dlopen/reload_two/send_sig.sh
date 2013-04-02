pid=`ps -ef |grep ./test |head -1 |awk '{print $2}'`
kill -4 ${pid}
