"""
Dwarf Beards
(c) 2014 Peter Denton
v 0.01
"""
#!/usr/bin/python

import pygame,sys,time,random
import numpy as np
from pygame.locals import *

seed=1888
rng=random.Random()
rng.seed(seed)

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

def p(s):
	print s

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
			self.condition_inc=(rng.random()+1)/50

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
		0	soil:		soft earth, will be initialized empty for now, can form hills in the future
		1	bedrock:	standard stone
		metals -
		2	silver
		3	gold
		4	iron
	properties are:
		diff:	difficulty to dig (0 = nothing, 1 = easy, 10 = quite hard)
		color
		objs:	list of objects contained, must be empty of lid!=0
		empty:	boolean, whether or not it is dug out or not, default is False
	"""
	lid_dict={
		0:("Soil",1,BROWN),
		1:("Bedrock",6,DARK_GRAY),
		2:("Silver",4,LIGHT_GRAY),
		3:("Gold",3,GOLD),
		4:("Iron",8,GRAY)
		}
	def __init__(self,lid):
		self.objs=[]
		self.empty=False
		self.lid=lid
		self.name=self.lid_dict[lid][0]
		self.diff=self.lid_dict[lid][1]
		self.color=self.lid_dict[lid][2]
	def change_kind(self,lid):
		self.lid=lid
		self.name=self.lid_dict[lid][0]
		self.diff=self.lid_dict[lid][1]
		self.color=self.lid_dict[lid][2]

class earth(object):
	def __init__(self,size):
		p("Creating world...")
		self.size=size
		self.earth=np.empty(self.size,dtype=location)
		for x in range(self.size[0]):
			for y in range(self.size[1]):
				# we note here that z=0 is the surface
				self.earth[x][y][0]=location(0)
				self.earth[x][y][0].empty=True
				# put bedrock everywhere else
				for z in range(1,self.size[2]):
					self.earth[x][y][z]=location(1)
		p("World created.")
	def seed_materials(self,num,lid):
		"""
		num:	how many locs should have the material
		lid:	see lid's from previous location class, should be a metal
		"""
		# assert location.lid_dict[lid] is metal
		if num==0: return
		start=(rng.randint(0,self.size[0]-1),rng.randint(0,self.size[1]-1),rng.randint(0,self.size[2]-1))
		while self.earth[start[0]][start[1]][start[2]].lid!=1:
			start=(rng.randint(0,self.size[0]-1),rng.randint(0,self.size[1]-1),rng.randint(0,self.size[2]-1))
		self.earth[start[0]][start[1]][start[2]]

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


