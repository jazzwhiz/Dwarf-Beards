"""
(c) 2014 Peter Denton
"""
#!/usr/bin/python

from world import rng,firsts,lasts
import tasks

class dwarf(object):
	def __init__(self,loc):
		self.beard=0
		# p(beard increasing +1) is between 1/10 and 2/10
		self.beard_inc=(rng.random()+1)/10.
		self.name="%s %s"%(rng.choice(firsts),rng.choice(lasts))
		# loc is an ordered pair (x,y,z)
		self.location=loc
		# initially should be idle
		self.task=tasks.task(self,0)
		# what to do after current action is done
		self.goal=None
		# an action is what is happening right now, and will always finish before the goal
		self.action=None
		self.thirst=0
		# thirst increases between 1/500 and 2/500, so it takes between 250,500 updates to become thirsty
		# maybe make this applied gaussian
		self.thirst_inc=(rng.random()+1)/500.
	def __str__(self):
		return "%s with beard length %i and is %s"%(self.name,self.beard,self.task)
		
	def beer(self):
		self.thirst=0
		if rng.random()<self.beard_inc:
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
#	def __init__(self):
#		return
	pass

