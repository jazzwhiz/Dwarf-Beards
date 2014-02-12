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

WHITE=(255,255,255)
BLACK=(0,0,0)
DARK_GRAY=(20,20,20)

def p(s):
	print s

def init_names():
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
	def __str__(self):
		s=self.name
		
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
	def __init__(self,tid):
		self.tid=tid
		

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

class locations(object):
	"""
	stuff in the earth
	kinds:
		empty:		can have stuff
		soil:		soft earth
		bedrock:	standard stone
		metals -
			silver
			gold
			iron
			
	"""
	def __init__(self,kind):
		self.kind=kind
		self.objs=[]
		if self.kind=="soil":
			self.diff=1
			

class earth(object):
	def __init__(self,size):
		self.earth=np.empty(size,dtype=locations)
		self.earth

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
		
		self.earth=earth((500,500,50))

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

	def buy_dwarf(self):
		p("Buying a dwarf...")
		self.dwarf_list.append(dwarf())
		p(self.dwarf_list[-1])
		pass

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
						self.buy_dwarf()
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()

			pygame.display.update()
			self.clock.tick(self.framerate)

world=World()
