# python
# -*- coding:cp936 -*-
import sys
from xml.sax import parse, handler, SAXException
import sets
import os
import getopt
import re



class Xml2txtHandler(handler.ContentHandler):
	def __init__(self,fout,fquery,tab):
		self.is_fromurl=0
		self.is_objurl=0
		self.is_query=0
		self.is_tab=0
		self.fromurl=""
		self.objurl=""
		self.query=""
		self.tab=""
		self.highrate_word=""
		self.need_tab=tab
		self.fout=fout
		self.fquery=fquery
		self.queryset=sets.Set()
		self.re_type_a = re.compile(r'[a-zA-Z]+\d+[a-zA-Z]*')
		self.re_type_b = re.compile(r'[a-zA-Z]*\d+[a-zA-Z]+')

	def startElement(self,name,attrs):
		if name == "fromurl":
			self.is_fromurl = 1
		elif name == "objurl":
			self.is_objurl = 1
		elif name == "query":
			self.is_query = 1
		elif name == "tab":
			self.is_tab= 1

	def endElement(self,name):
		if name == "fromurl":
			self.is_fromurl = 0
			self.fromurl = self.fromurl.strip()
		elif name == "objurl":
			self.is_objurl = 0
			self.objurl = self.objurl.strip()
		elif name == "query":
			self.is_query = 0
			toks=self.query.strip().split("$$")
			self.query=' '.join(toks)


			worddict={}
			for tok in toks:
				self.queryset.add(tok)
				res=self.re_type_a.findall(tok)
				for seg in res:
					worddict[seg.lower()] = worddict.setdefault(seg.lower(),0) + 1
				res=self.re_type_b.findall(tok)
				for seg in res:
					worddict[seg.lower()] = worddict.setdefault(seg.lower(),0) + 1

			toks=self.query.strip().split(" ")
			tok_set = sets.Set()
			for tok in toks:
				tok_set.add(tok)
			tok_list=[]
			for tok in tok_set:
				tok_list.append(tok)
			self.query=' '.join(tok_list)

			max=0
			for (k,v) in worddict.iteritems():
				if v > max and k != " ":
					self.highrate_word = k
					max = v

		elif name == "tab":
			self.is_tab = 0
			if self.tab.strip() == self.need_tab:
				self.fout.write(self.fromurl+"\t")
				self.fout.write(self.objurl+"\t")
				self.fout.write(self.query.encode('cp936')+"\n")
				#self.queryset.add(self.highrate_word)
			self.fromurl=""
			self.objurl=""
			self.query=""
			self.tab=""
			self.highrate_word=""


	def characters(self, content):
		if self.is_fromurl == 1:
			self.fromurl+= content
		elif self.is_objurl ==1:
			self.objurl += content
		elif  self.is_query == 1:
			self.query += content
		elif self.is_tab == 1:
			self.tab += content

	def endDocument(self):
		for tok in self.queryset:
			self.fquery.write(tok.encode('cp936')+'\n')


if __name__ == '__main__':
	f=""
	tab="1"
	opts,args=getopt.getopt(sys.argv[1:],"i:t:")
	for arg,opt in opts:
		if (arg == "-i"):
			f=opt
		if (arg == "-t"):
			tab=opt

	if f == "":
		exit(1)

	fout=open("out.txt",'w')
	fquery=open("query",'w')

	try:
		    parse(f, Xml2txtHandler(fout,fquery,tab))
	except SAXException, msg:
		    print msg.getException()
	'''
	except:
		    print sys.exc_info()[0],sys.exc_info()[1]
	'''

