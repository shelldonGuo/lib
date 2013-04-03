# python
# -*- coding:cp936 -*-
import sys
sys.path.append( './' )

import wordseg
import xml.dom.minidom 
from xml.dom import Node
import sets
import os
import getopt

dict_handle = wordseg.loadDict('./worddict/')
result_handle = wordseg.createResult()

def xml2txt(xml_file,fout,fquery):
	doc = xml.dom.minidom.parse(xml_file)  
	queryset=sets.Set()
	for node in doc.getElementsByTagName("item"):
		fromurl=""
		objurl=""
		query=""
		wordset=sets.Set()
		wordlist=[]
		for child in node.childNodes:
			if child.nodeType == Node.ELEMENT_NODE:
				if child.nodeName == "fromurl":
					fromurl=child.firstChild.nodeValue
				elif child.nodeName == "objurl":
					objurl=child.firstChild.nodeValue
				elif child.nodeName == "query":
					query=child.firstChild.nodeValue
					toks=query.split("$$")
					for tok in toks:
						queryset.add(tok.encode('cp936'))
						segs = wordseg.getSegment( dict_handle , result_handle , wordseg.SCW_BASIC, tok.encode('cp936'))
						for seg in segs:
							wordset.add(seg)
					for seg in wordset:
						wordlist.append(seg)
					query=' '.join(wordlist)
		fout.write(fromurl+"\t")
		fout.write(objurl+"\t")
		fout.write(query+"\n")

	for tok in queryset:
		fquery.write(tok+'\n')

f=""
opts,args=getopt.getopt(sys.argv[1:],"i:")
for arg,opt in opts:
	if (arg == "-i"):
		f=opt

if f == "":
	exit(1)

fout=open("out.txt",'w')
fquery=open("query",'w')
xml2txt(f,fout,fquery)

#os.system('sort -u query > query.out')

wordseg.destroyDict( dict_handle )
