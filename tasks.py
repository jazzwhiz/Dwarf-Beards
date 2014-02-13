"""
(c) 2014 Peter Denton
"""
#!/usr/bin/python

from world import rng

class task(object):
	"""
	requires a task id which is also it's priority
	0: Idle (lowest priority)
	1: Walk
	2: Dig	(horizontal)
	3: Mine	(down)
	4: Eat
	5: Drink
	6: Sleep (highest priority)
	"""
	tid_dict={
		0:"Idle",
		1:"Walk",
		2:"Dig",
		3:"Mine",
		4:"Eat",
		5:"Drink",
		6:"Sleep"
		}
	def __str__(self):
		if self.name[-1]=="e":
			return "%sing"%self.name[:-1]
		else:
			return "%sing"%self.name
	def move(self):
		"""
		returns a delta, must be normed to one
		"""
		return (0,0,0)

class idle(task):
	"""
	pace on the same floor only
	"""
	def __init__(self,earth,start,wait):
		diffs=[(1,0,0),(-1,0,0),(0,1,0),(0,-1,0)]

		self.earth=earth
		self.tid=0
		self.name=self.tid_dict[self.tid]
		self.start=start
		self.loc=start
		self.wait=wait
		self.waited=0

		# check if possible to wander or not
		self.stuck=True
		for diff in diffs:
			if self.earth.earth[self.loc[0]+diff[0]][self.loc[1]+diff[1]][self.loc[2]+diff[2]].empty:
				self.stuck=False

		# pick a direction at random
		if not self.stuck:
			rng.shuffle(diffs)
			for diff in diffs:
				if self.earth.earth[self.loc[0]+diff[0]][self.loc[1]+diff[1]][self.loc[2]+diff[2]].empty:
					self.diff=diff
					break

	def update(self,loc):
		"""
		updates the current location as necessary
		"""
		self.loc=loc
	def move(self):
		if not self.stuck:
			self.waited+=1
			if self.waited>self.wait:
				self.waited=0
				if self.loc==self.start:
					return self.diff
				else:
					return tuple(-x for x in self.diff)
			else:
				return (0,0,0)

