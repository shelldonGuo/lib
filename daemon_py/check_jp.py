#python
# -*- coding: utf-8 -*-

import re
import urllib2
import time
import os
import sys

def daemonize(stdin='/dev/null', stdout='/dev/null', stderr='/dev/null'):
    # Perform first fork.
    try:
        pid = os.fork()
        if pid > 0:
            sys.exit(0) # Exit first parent.
    except OSError, e:
        sys.stderr.write("fork #1 failed: (%d) %sn" % (e.errno, e.strerror))
        sys.exit(1)
    # Decouple from parent environment.
    os.chdir("/")
    os.umask(0)
    os.setsid()
    # Perform second fork.
    try:
        pid = os.fork()
        if pid > 0:
            sys.exit(0) # Exit second parent.
    except OSError, e:
        sys.stderr.write("fork #2 failed: (%d) %sn" % (e.errno, e.strerror))
        sys.exit(1)
    # The process is now daemonized, redirect standard file descriptors.
    for f in sys.stdout, sys.stderr:
        f.flush()
    si = file(stdin, 'r')
    so = file(stdout, 'a+')
    se = file(stderr, 'a+', 0)
    os.dup2(si.fileno(), sys.stdin.fileno())
    os.dup2(so.fileno(), sys.stdout.fileno())
    os.dup2(se.fileno(), sys.stderr.fileno())



def check():
	target=6
	TIME_RETRY=2*60
	TIME_CLC=10*60
	log=open("/tmp/check_jp.log",'w')

	'''
	TIME_RETRY=2*60
	TIME_CLC=2
	'''
	re_jp=re.compile('<td bgcolor="#FFFFFF">\xe6\x97\xa5\xe5\x85\x83</td>.*?<td bgcolor="#FFFFFF">(.*?)</td>.*?<td bgcolor="#FFFFFF">(.*?)</td>.*?<td bgcolor="#FFFFFF">(.*?)</td>.*?<td bgcolor="#FFFFFF">(.*?)</td>.*?<td bgcolor="#FFFFFF">(.*?)</td>.*?<td bgcolor="#FFFFFF">(\d\d\d\d-\d\d-\d\d)</td>.*?<td bgcolor="#FFFFFF">(\d\d:\d\d:\d\d)</td>',re.S)
	url="http://www.boc.cn/sourcedb/whpj/"

	first_flag=1
	i=0
	while True:
		response = urllib2.urlopen(url)
		html = response.read()
		res=re_jp.search(html,re.S)
		if res == None:
			log.write("parse html error\n")
			log.flush()
			time.sleep(TIME_RETRY)
			continue

		ratio=res.group(1)
		ratio_now=int((float(ratio)-7)*10)

		time_all=res.group(7)
		time_toks=time_all.split(":")
		hour=int(time_toks[0])
		minu=int(time_toks[1])
		time_now=hour*60+minu

		tm=time.localtime()
		tm_hour=tm.tm_hour
		tm_min=tm.tm_min
		if (tm_hour >= 17 ):
			TIME_NIGHT=9*60*60+(24-tm_hour)*60*60-tm_min*60
			log.write("sleep for [%d] sec\n"%(TIME_NIGHT))
			log.flush()
			time.sleep(TIME_NIGHT)
			continue
		elif (tm_hour < 9):
			TIME_NIGHT=(9-tm_hour)*60*60-tm_min*60
			log.write( "sleep for [%d] sec\n"%(TIME_NIGHT))
			log.flush()
			time.sleep(TIME_NIGHT)
			continue

		if (first_flag == 1):
			last_time=time_now
			last_ratio=ratio_now
			first_flag=0
			continue

		if (time_now != last_time and ratio_now != last_ratio and ratio_now >= target):
			log.write( "Ratio is gt 6!!!\n"%(ratio_now))
			log.flush()
			os.system('/bin/gsmsend -s sys-monitor.vm:15002 15212345678@"OK"')
			break

		log.write( "cycle [%d],ratio=[%s]\n"%(i,ratio))
		log.flush()
		i+=1
		time.sleep(TIME_CLC)


if __name__ == '__main__':
	daemonize(stdout='/tmp/stdout.log', stderr='/tmp/stderr.log')
	check()
