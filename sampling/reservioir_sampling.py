#!/usr/bin/python

import sys
import random

#------------------
# config
#------------------
need=2000
#------------------

res=[]
line_num=0

for line in sys.stdin:
	line=line.rstrip()
	if (line_num < need):
		res.append(line)
	else:
		k=random.randint(1,line_num)-1
		if (k < need):
			res[k]=line

	line_num+=1

for line in res:
	print line
