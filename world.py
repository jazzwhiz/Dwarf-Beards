"""
Dwarf Beards
(c) 2014 Peter Denton
v 0.01
"""
#!/usr/bin/python

import pygame,sys,random
import numpy as np
from pygame.locals import *

# initialize rng
seed=1888
rng=random.Random()
rng.seed(seed)

# Names
firsts=[]
lasts=[]

import dwarves,tasks

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
RED=(255,0,0)
DARK_BLUE=(0,0,100)

class obj(object):
	"""
	things that are at locations
	0:	earth:			stuff that was dug out of the earth
	1:	axe:			digs in anything
	2:	shovel:			cheaper than axe, digs in soil only
	3:	cart:			hauls one half of one location away
	4:	ladder:			allows for movement vertically
	5:	trading post:	turn resources into moneys
	"""
	oid_dict={
		0:("Earth"),
		1:("Axe"),
		2:("Shovel"),
		3:("Cart"),
		4:("Ladder"),
		5:("Trading post")
		}
	def __init__(self,oid):
		self.oid=oid
		self.name=self.oid_dict[self.oid]
		self.condition=1
		self.condition_inc=0
		if self.oid==1:
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

class rock(obj):
	def __init__(self,lid):
		self.oid=0
		self.lid=lid
		self.condition=1
		self.condition_inc=0
		self.name="Pile of %s"%location.lid_dict[lid]

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
		for lid in range(2,5):
			for num_veins in range(rng.randint(2,6)):
				self.seed_materials(rng.randint(25,100),lid)
	def seed_materials(self,num,lid):
		"""
		num:	how many locs should have the material
		lid:	see lid's from previous location class, should be a metal
		"""
		# assert location.lid_dict[lid] is metal
		if num==0: return
		start=(rng.randint(0,self.size[0]-1),rng.randint(0,self.size[1]-1),rng.randint(0,self.size[2]-1))
		metal_locs=[]
		while self.earth[start[0]][start[1]][start[2]].lid!=1:
			start=(rng.randint(0,self.size[0]-1),rng.randint(0,self.size[1]-1),rng.randint(0,self.size[2]-1))
		metal_locs.append(start)
		diffs=[(1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,1),(0,0,-1)]
		while len(metal_locs)<num:
			focus=rng.choice(metal_locs)
			tmp_diffs=rng.sample(diffs,rng.randint(1,6))
			tmp_locs=[tuple(sum(x) for x in zip(focus,diff)) for diff in tmp_diffs]
			for tmp_loc in tmp_locs:
				if tmp_loc[0]>=0 and tmp_loc[0]<self.size[0]:
					if tmp_loc[1]>=0 and tmp_loc[1]<self.size[1]:
						if tmp_loc[2]>=0 and tmp_loc[2]<self.size[2]:
							if self.earth[tmp_loc[0]][tmp_loc[1]][tmp_loc[2]].lid==1:
								if tmp_loc not in metal_locs:
									metal_locs.append(tmp_loc)
		# in case we accidentally added too many on the last step
		while len(metal_locs)>num:
			metal_locs.pop()
		for loc in metal_locs:
			self.earth[loc[0]][loc[1]][loc[2]]=location(lid)

class World(object):
	def __init__(self):
		self.screen_size=(800,600)
		self.screen=pygame.display.set_mode(self.screen_size)
		self.clock=pygame.time.Clock()
		self.framerate=60
		self.bg_color=BLACK
		self.screen.fill(self.bg_color)
		self.dwarf_list=[]
		self.task_queue=[]
		self.notifications=[]

		pygame.key.set_repeat(200,50)

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
			self.clock.tick(self.framerate)
		self.screen.fill(BLACK)
		pygame.display.update()
		self.clock.tick(self.framerate)
		
		self.p("Reading names into memory...")
		self.init_names()
		self.p("Names read into memory.")

		self.p("Creating world...")
		self.earth_size=(50,50,11)
		self.earth=earth(self.earth_size)

		# initial level (0 = top, 10... is bottom)
		self.level=0
		# focus
		self.focus=(self.earth_size[0]/2,self.earth_size[1]/2)
		self.focus_loc=self.earth.earth[self.focus[0]][self.focus[1]][self.level]
		self.p("World created.")

		self.run()


	def p(self,s):
		print s

	def notify(self,s):
		"""
		todo:
		make this work
		"""
		self.notifications.append(s)
		self.p(s)

	def init_names(self,):
		# set up names
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

	def help(self):
		self.screen.fill(BLACK)

		y=20

		self.text("esc",20,WHITE,(20,y))
		self.text("- Exit",20,WHITE,(60,y))
		y+=20
		self.text("h",20,WHITE,(20,y))
		self.text("- Toggle this help screen",20,WHITE,(60,y))
		y+=20
		self.text("n",20,WHITE,(20,y))
		self.text("- Buy a new dwarf",20,WHITE,(60,y))
		y+=20
		self.text("d",20,WHITE,(20,y))
		self.text("- Dig (horizontal)",20,WHITE,(60,y))
		y+=20
		self.text("m",20,WHITE,(20,y))
		self.text("- Mine (down)",20,WHITE,(60,y))
		y+=20
		self.text("<",20,WHITE,(20,y))
		self.text("- Up one z level",20,WHITE,(60,y))
		y+=20
		self.text(">",20,WHITE,(20,y))
		self.text("- Down one z level",20,WHITE,(60,y))


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
			self.clock.tick(self.framerate)

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

	def buy_dwarf(self,loc):
		"""
		todo:	take out funds
		"""
		self.p("Buying a dwarf...")
		self.dwarf_list.append(dwarves.dwarf(self.earth,loc))
		self.p("Bought dwarf %s"%self.dwarf_list[-1])

	def draw(self):
		"""
		todo:	draw all the stuff
		"""
		self.screen.fill(BLACK)
		
		# draw the earth
		for x in range(self.earth_size[0]):
			for y in range(self.earth_size[1]):
				# todo: draw dotted pattern for floor
				pygame.draw.rect(self.screen,self.earth.earth[x][y][self.level].color,(12*x+1,12*y+1,10,10),0)
		for task in self.task_queue:
			if task.loc[2]==self.level:
				# digging down
				if task.tid==2:
					pygame.draw.line(self.screen,BLACK,(12*task.loc[0]+10,12*task.loc[1]+1),(12*task.loc[0]+1,12*task.loc[1]+10),2)
					pygame.draw.line(self.screen,BLACK,(12*task.loc[0]+1,12*task.loc[1]+1),(12*task.loc[0]+10,12*task.loc[1]+10),2)
				# mining out
				if task.tid==3:
					pygame.draw.line(self.screen,BLACK,(12*task.loc[0]+10,12*task.loc[1]+1),(12*task.loc[0]+1,12*task.loc[1]+10),2)

		# draw the dwarves
		for dwarf in self.dwarf_list:
			if dwarf.loc[2]==self.level:
				pygame.draw.circle(self.screen,BLACK,(12*dwarf.loc[0]+6,12*dwarf.loc[1]+6),5,0)

		# draw the focus box
		focus_corners=[
			(12*self.focus[0],12*self.focus[1]),
			(12*(self.focus[0]+1),12*self.focus[1]),
			(12*(self.focus[0]+1),12*(self.focus[1]+1)),
			(12*self.focus[0],12*(self.focus[1]+1))
			]
		for i in range(4):
			pygame.draw.line(self.screen,WHITE,focus_corners[i],focus_corners[(i+1)%4],2)

		# now we draw RHS of stuff
		self.text("(%i,%i,%i)"%(self.focus[0],self.focus[1],-self.level),15,RED,(610,5))

		if self.focus_loc.empty:
			self.text("%s floor"%self.focus_loc.name,15,RED,(680,5))
		else:
			self.text(self.focus_loc.name,15,RED,(680,5))

		y=25
		for dwarf in self.dwarf_list:
			if dwarf.loc==self.focus+(self.level,):
				self.text("%s (%i)"%(dwarf.name,dwarf.beard),15,LIGHT_GRAY,(625,y))
				y+=16
		for obj in self.focus_loc.objs:
			self.text(obj.name,15,DARK_BLUE,(625,y))
			y+=16

	def update(self):
		self.focus_loc=self.earth.earth[self.focus[0]][self.focus[1]][self.level]
		for dwarf in self.dwarf_list:
			dwarf.update()

	def run(self):
		while True:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:

					# general game action
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.key==ord('h'):
						self.help()

					# dwarf action
					if event.key==ord('n'):
						if self.focus_loc.empty:
							self.buy_dwarf(self.focus+(self.level,))
						else:
							self.notify("Cannot place dwarf here.")

					# adding tasks
					if event.key==ord('d'):
						if self.level>=self.earth_size[2]-1:
							self.notify("If you dig any deeper you'll hit magma, better not...")
						elif self.earth.earth[self.focus[0]][self.focus[1]][self.level+1].empty:
							self.notify("It is already empty beneath here.")
						else:
							something=False
							for task in self.task_queue:
								if task.tid in [2,3]:
									if task.loc==self.focus+(self.level,):
										self.notify("We are already %s here."%task.lower())
										something=True
							if not something:
								self.task_queue.append(tasks.dig(self.earth,self.focus+(self.level,),0))
					if event.key==ord('m'):
						if self.focus_loc.empty:
							self.notify("It is already empty here.")
						else:
							something=False
							for task in self.task_queue:
								if task.tid in [2,3]:
									if task.loc==self.focus+(self.level,):
										self.notify("We are already %s here."%task.lower())
										something=True
							if not something:
								self.task_queue.append(tasks.mine(self.earth,self.focus+(self.level,),0))

					# move focus in the z direction
					if event.key==ord(',') and pygame.key.get_mods() in [1,2,3]:
#						self.p("Moving out of the earth one level")
						self.level=max(self.level-1,0)
					if event.key==ord('.') and pygame.key.get_mods() in [1,2,3]:
#						self.p("Moving into the earth one level")
						self.level=min(self.level+1,self.earth_size[2]-1)

					# move focus in the plane, modularly (I'm not a monster)
					mod=1
					if pygame.key.get_mods() in [1,2,3]:
						mod=10
					if event.key==K_UP:
						self.focus=(self.focus[0],(self.focus[1]-mod)%self.earth_size[1])
					if event.key==K_DOWN:
						self.focus=(self.focus[0],(self.focus[1]+mod)%self.earth_size[1])
					if event.key==K_LEFT:
						self.focus=((self.focus[0]-mod)%self.earth_size[0],self.focus[1])
					if event.key==K_RIGHT:
						self.focus=((self.focus[0]+mod)%self.earth_size[0],self.focus[1])

				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
			self.update()
			self.draw()
			pygame.display.update()
			self.clock.tick(self.framerate)


