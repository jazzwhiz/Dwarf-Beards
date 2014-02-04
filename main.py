#!/usr/bin/python

import pygame,dwarves,time,random
import numpy as np
from pygame.locals import *

pygame.init()

WHITE=(255,255,255)
BLACK=(0,0,0)

class locations(object):
	# stuff in the earth
	def __init__(self,kind,solid=True):
		self.kind=kind
		self.solid=solid
		if not solid:
			self.objects=[]
			self.dwarves=[]

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
					return
		self.screen.fill(BLACK)
		pygame.display.update()
		

		self.earth=earth((500,500,50))

		self.run()

	def help(self):
		self.screen.fill(BLACK)
		self.text(

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
					if event.key==ord('h')
						self.help()
				if event.type==pygame.QUIT:
					return

			pygame.display.update()
			self.clock.tick(self.framerate)

world=World()
