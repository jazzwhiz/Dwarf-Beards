"""
Dwarf Beards
(c) 2014 Peter Denton
v 0.01
"""
#!/usr/bin/python

import pygame,sys,time,random
import numpy as np
from pygame.locals import *

pygame.init()

# Some colors
# Grayscale
WHITE=(255,255,255)
LIGHT_GRAY=(100,100,100)
GRAY=(60,60,60)
DARK_GRAY=(20,20,20)
BLACK=(0,0,0)

# Others
BROWN=(139,69,19)
GOLD=(255,215,0)

# Name lists
firsts=[]
lasts=[]

def p(s):
	print s

def init_names():
	# set up names
	p("Reading names into memory...")
	first=open("first.txt","r")
	for line in first.readlines():
		line=line.replace("\n","")
		if line=="":
			continue
		firsts.append(line)
	first.close()
	
	last=open("last.txt","r")
	for line in last.readlines():
		line=line.replace("\n","")
		if line=="":
			continue
		lasts.append(line)
	last.close()
	p("Names read into memory.")

class dwarf(object):
	def __init__(self,loc):
		self.beard=0
		# p(beard increasing +1) is between 1/10 and 2/10
		self.beard_inc=(random.random()+1)/10.
		self.name="%s %s"%(random.choice(firsts),random.choice(lasts))
		# loc is an ordered pair (x,y,z)
		self.location=loc
		# initially should be idle
		self.task=task(self,0)
		# what to do after current action is done
		self.goal=None
		# an action is what is happening right now, and will always finish before the goal
		self.action=None
		self.thirst=0
		# thirst increases between 1/500 and 2/500, so it takes between 250,500 updates to become thirsty
		# maybe make this applied gaussian
		self.thirst_inc=(random.random()+1)/500.
	def __str__(self):
		return "%s with beard length %i and is %s"%(self.name,self.beard,self.task)
		
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
#	def __init__(self):
#		return
	pass

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

class objs(object):
	"""
	things that are at locations
	axe:	digs in anything
	shovel:	cheaper than axe, digs in soil only
	cart:	hauls one half of one location away
	ladder: allows for movement vertically
	"""
	def __init__(self,kind):
		self.kind=kind
		self.condition=1
		if self.kind=="axe":
			# will last between 25 and 50 uses
			self.condition_inc=(random.random()+1)/50

	def use(self):
		self.condition-=self.condition_inc
		if self.condition<=0:
			self.destroy()
	def repair(self):
		self.condition=1
	def destroy(self):
		self.__del__(self)

class location(object):
	"""
	stuff in the earth
	kinds:
		0	empty:		can have stuff
		1	soil:		soft earth
		2	bedrock:	standard stone
		metals -
		3	silver
		4	gold
		5	iron
	properties are:
		diff:	difficulty to dig (0 = nothing, 1 = easy, 10 = quite hard)
		color
			
	"""
	lid_dict={
		0:("Empty",0,BLACK),
		1:("Soil",1,BROWN),
		2:("Bedrock",6,DARK_GRAY),
		3:("Silver",4,LIGHT_GRAY),
		4:("Gold",3,GOLD),
		5:("Iron",8,GRAY)
		}
	def __init__(self,lid):
		self.objs=[]
		self.lid=lid
		self.name=self.lid_dict[lid][0]
		self.diff=self.lid_dict[lid][1]
		self.color=self.lid_dict[lid][2]
			

class earth(object):
	def __init__(self,size):
		p("Creating world...")
		self.earth=np.empty(size,dtype=location)
		for x in range(size[0]):
			for y in range(size[1]):
				for z in range(size[2]):
					self.earth[x][y][z]=location(0)
		p("World created.")

class World(object):
	def __init__(self):
		self.screen_size=(800,600)
		self.screen=pygame.display.set_mode(self.screen_size)
		self.clock=pygame.time.Clock()
		self.framerate=60
		self.bg_color=BLACK
		self.screen.fill(self.bg_color)
		self.dwarf_list=[]

		init_names()

		self.text("Dwarf Beards",90,WHITE,location=(0,200),centerx=True)
		self.text("created by",20,WHITE,location=(0,350),centerx=True)
		self.text("Peter Denton",50,WHITE,location=(0,370),centerx=True)
		self.text("Press enter to begin",30,WHITE,location=(0,500),centerx=True)
		pygame.display.update()

		titling=True
		while titling:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.key==K_RETURN:
						titling=False
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
		self.screen.fill(BLACK)
		pygame.display.update()
		self.clock.tick(self.framerate)
		
		self.earth=earth((50,50,5))

		self.run()

	def help(self):
		self.screen.fill(BLACK)

		y=20

		self.text("esc",20,WHITE,(20,y))
		self.text("- Exit",20,WHITE,(60,y))
		y+=20
		self.text("h",20,WHITE,(20,y))
		self.text("- Toggle this help screen",20,WHITE,(60,y))
		y+=20
		self.text("m",20,WHITE,(20,y))
		self.text("- Buy a miner",20,WHITE,(60,y))


		pygame.display.update()
		self.clock.tick(self.framerate)

		helping=True
		while helping:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.key==ord('h'):
						helping=False
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()


	def text(self,msg,size,color,location=(0,0),centerx=False,centery=False):
		f=pygame.font.SysFont(None,size)
		t=f.render(msg,True,color)
		tRect=t.get_rect()
		tRect.left=location[0]
		tRect.top=location[1]
		if centerx:
			tRect.centerx=self.screen.get_rect().centerx
		if centery:
			tRect.centery=self.screen.get_rect().centery
		self.screen.blit(t,tRect)

	def buy_miner(self,loc):
		"""
		todo:	where does miner go?
				take out funds
		"""
		p("Buying a dwarf...")
		self.dwarf_list.append(miner(loc))
		p("Bought dwarf %s"%self.dwarf_list[-1])

	def draw(self):
		"""
		todo:	draw all the stuff
		"""
		self.screen.fill(BLACK)

	def run(self):
		while True:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.key==ord('h'):
						self.help()
					if event.key==ord('m'):
						self.buy_miner((0,0,0))
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
			self.draw()
			pygame.display.update()
			self.clock.tick(self.framerate)

world=World()
