#!/usr/bin/python
# -*- coding: UTF8 -*-
import sys
import copy

def init_pass(T):
	C = {}
	for t in T:
		for i in t:
			if i in C.keys():
				C[i] += 1
			else:
				C[i] = 1
	return C

def candidate_gen(F):
	C = []
	k = len(F[0])
	for f1 in F:
		for f2 in F:
			if f1[-1] < f2[-1]:
				c = copy.copy(f1)
				c.append(f2[-1])
				flag = True
				for i in range(0,k):
					if i == 0:
						s=c[1:]
					elif i == k-1:
						s=c[:k-1]
					else:
						s = c[0:i]+c[i:]
					if s not in F:
						flag = False
						break
				if flag and c not in C:
					C.append(c)
	return C

def compare_list(A,B):
	if len(A) <= len(B):
		for a in A:
			if a not in B:
				return False
	else:
		for b in B:
			if b not in A:
				return False
	return True
	
def apriori(T, minsup):
	C = []
	init = init_pass(T)
	keys = init.keys()
	keys.sort()
	C=keys
	n = len(T)
	F = [[]]
	for f in C:
		if init[f]*1.0/n >= minsup:
			F[0].append([f])
	k = 1
	while F[k-1] != []:
		C=candidate_gen(F[k-1])
		F.append([])
		for c in C:
			count = 0;
			for t in T:
				if compare_list(c,t):
					count += 1
			if count*1.0/n >= minsup:
				F[k].append(c)
		k += 1
	U = []
	for f in F:
		for x in f:
			U.append(x)
	return U
def read():
	fp=open("0",'r')
	T = []
	for line in fp:
		line=line.rstrip()
		toks=line.split("\t")
		fs=toks[4].split(" ")
		T.append(fs)
	return T
def f5(seq, idfun=None): 
	# order preserving
	if idfun is None:
		def idfun(x): return x
	seen = {}
	result = []
	for item in seq:
		marker = idfun(item)
		# in old Python versions:
		# if seen.has_key(marker)
		# but in new ones:
		if marker in seen: continue
		seen[marker] = 1
		result.append(item)
	return result
T = read()
#T = [['A','B','B','C','D'],['B','C','E'],['A','B','C','E'],['B','D','E'],['A','B','C','D']]
for i in range(0,len(T)):
	T[i]=f5(T[i])
#print T

F = apriori(T, 0.3)
for f in F:
	for i in f:
		print i,
	print 
