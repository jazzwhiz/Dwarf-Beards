"""
(c) 2014 Peter Denton
"""
#!/usr/bin/python

from world import rng,firsts,lasts
import tasks

class dwarf(object):
	def __init__(self,earth,loc):
		self.earth=earth
		self.beard=0
		# p(beard increasing +1) is between 1/10 and 2/10
		self.beard_inc=(rng.random()+1)/10.
		self.name="%s %s"%(rng.choice(firsts),rng.choice(lasts))
		# loc is an ordered pair (x,y,z)
		self.loc=loc
		# initially should be idle, idle_time is a measure of impatience
		self.idle_time=rng.randint(40,200)
		self.task=tasks.idle(self.earth,self.loc,self.idle_time)
		# what to do after current action is done
		self.goal=None
		# an action is what is happening right now, and will always finish before the goal
		self.action=None
		self.thirst=0
		# thirst increases between 1/500 and 2/5000, so it takes between 2500,5000 updates to become thirsty
		# maybe make this applied gaussian
		self.thirst_inc=(rng.random()+1)/5000.
	def __str__(self):
		return "%s with beard length %i and is %s"%(self.name,self.beard,self.task)
		
	def beer(self):
		self.thirst=0
		if rng.random()<self.beard_inc:
			self.beard+=1

	def update(self):
		# do internal things first
		self.thirst+=self.thirst_inc
		if self.thirst>=1:
			self.goal==tasks.drink(self.earth)

		# update current task
		self.task.update(self.loc)

		self.loc=tuple(sum(x) for x in zip(self.loc,self.task.move()))


		# update goals
		# beer has highest priority

		# turn goals into actions
		if self.action==None and self.goal!=None:
			self.action=self.goal
			

