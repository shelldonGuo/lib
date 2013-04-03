#!/usr/bin/python
# ret :a & b

import sys

objurl_last=""
objurl=""
set_a_flag=False
set_b_flag=False

for line in sys.stdin:
	line=line.rstrip()
	toks=line.split("\t")
	if(len(objurl_last)<=0):
		objurl_last=toks[0]

	objurl=toks[0]

	if(objurl != objurl_last):
		if (set_a_flag == True and set_b_flag == True):
			print "%s"%(objurl_last)
		objurl_last=objurl
		set_a_flag=False
		set_b_flag=False

	if (toks[1] == 'a'):
		set_a_flag=True
	elif (toks[1] == 'b'):
		set_b_flag=True

if (set_a_flag == True and set_b_flag == True):
	print "%s"%(objurl)
