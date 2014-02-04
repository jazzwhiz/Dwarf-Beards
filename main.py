"""
Dwarf Beards
(c) 2014 Peter Denton
"""
#!/usr/bin/python

import pygame,sys,dwarves,time,random
import numpy as np
from pygame.locals import *

pygame.init()

WHITE=(255,255,255)
BLACK=(0,0,0)
DARK_GRAY=(20,20,20)

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
			self.diff=
			

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
						self.
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()

			pygame.display.update()
			self.clock.tick(self.framerate)

world=World()
