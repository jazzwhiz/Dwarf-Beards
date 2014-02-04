#!/usr/bin/python

from random import choice

# set up names
first=open("first.txt","r")
firsts=[]
for line in first.readlines():
	line.remove("\n")
	if line=="":
		continue
	firsts.append(line)
first.close()

last=open("last.txt","r")
lasts=[]
for line in last.readlines():
	line.remove("\n")
	if line=="":
		continue
	lasts.append(line)
last.close()

class dwarf(object):
	def __init__(self):
		self.beard=0
		self.name="%s %s"%(choice(firsts),choice(lasts))

class miner(dwarf):
	def __init__(self):
		return
