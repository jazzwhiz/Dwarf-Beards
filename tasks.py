"""
(c) 2014 Peter Denton
"""
#!/usr/bin/python

class task(object):
	"""
	requires a task id which is also it's priority
	0: Idle (lowest priority)
	1: Walk
	2: Dig
	3: Eat
	4: Drink
	5: Sleep (highest priority)
	"""
	tid_dict={
		0:"Idle",
		1:"Walk",
		2:"Dig",
		3:"Eat",
		4:"Drink",
		5:"Sleep"
		}
	def __init__(self,dwarf,tid):
		self.tid=tid
		self.dwarf=dwarf
		self.name=self.tid_dict[tid]
	def __str__(self):
		if self.tid==0:
			return "Idling"
		else:
			return "%sing"%self.name

	def act(self):
		if tid==0:
			self.idle()
		if tid==1:
			self.move()
		if tid==2:
			self.dig()
		if tid==3:
			self.eat()
		if tid==4:
			self.drink()
		if tid==5:
			self.sleep()

	def idle(self):
		pass
	def move(self):
		pass
	def dig(self):
		pass
	def eat(self):
		pass
	def drink(self):
		pass
	def sleep(self):
		pass

