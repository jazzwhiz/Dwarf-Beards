#!/usr/bin/python

import random

# set up names
first=open("first.txt","r")
firsts=[]
for line in first.readlines():
	line=line.replace("\n","")
	if line=="":
		continue
	firsts.append(line)
first.close()

last=open("last.txt","r")
lasts=[]
for line in last.readlines():
	line=line.replace("\n","")
	if line=="":
		continue
	lasts.append(line)
last.close()

class dwarf(object):
	def __init__(self):
		self.beard=0
		# p(beard increasing +1) is between 1/10 and 2/10
		self.beard_inc=(random.random()+1)/10.
		self.name="%s %s"%(random.choice(firsts),random.choice(lasts))
		self.location=(0,0)
		# where to move to
		self.target=None
		# what to do after current action is done
		self.goal=None
		# an action is what is happening right now, and will always finish before the goal
		self.action=None
		self.thirst=0
		# thirst increases between 1/500 and 2/500, so it takes between 250,500 updates to become thirsty
		# maybe make this applied gaussian
		self.thirst_inc=(random.random()+1)/500.
	def beer(self):
		self.thirst=0
		if random.random()<self.beard_inc:
			self.beard+=1
	def update(self):
		self.thirst+=self.thirst_inc

		# update goals
		# beer has highest priority
		if self.thirst>=1:
			self.goal=="beer"

		# turn goals into actions
		if self.action==None and self.goal!=None:
			self.action=self.goal
			

class miner(dwarf):
	def __init__(self):
		return
