"""
Dwarf Beards
(c) 2014 Peter Denton
"""
#!/usr/bin/python

__version__="0.01"
copyright="(c) 2014 Peter Denton"

import pygame,sys,random
import numpy as np
from pygame.locals import *
import jps,Astar

# initialize rng
seed=1888
rng=random.Random()
rng.seed(seed)

# Names
firsts=[]
lasts=[]

# Some colors
# Grayscale
WHITE=(255,255,255)
LIGHT_GRAY=(150,150,150)
GRAY=(90,90,90)
DARK_GRAY=(60,60,60)
BLACK=(0,0,0)

# Others
BROWN=(139,69,19)
GOLD=(255,215,0)
RED=(255,0,0)
DARK_BLUE=(0,0,100)
YELLOW=(255,255,0)

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
		self.task=idle(self.earth,self.loc,self.idle_time)
		# what to do after current action is done
		self.goal=None
		self.thirst=0
		# thirst increases between 1/500 and 2/5000, so it takes between 2500,5000 updates to become thirsty
		self.thirst_inc=(rng.random()+1)/5000.
		# laziness is a prob that a task is accepted out of idle
		self.laziness=rng.random()/10.
		# how fast?
		self.walk_wait=int(self.idle_time/(2+rng.random()))
	def __str__(self):
		return "%s with beard length %i and is %s"%(self.name,self.beard,self.task)

	"""	
	def beer(self):
		self.thirst=0
		if rng.random()<self.beard_inc:
			self.beard+=1
	"""

	def update(self):
		# do internal things first
		self.thirst+=self.thirst_inc
		if self.thirst>=1:
			print "%s is thirsty..."%self.name
			self.goal==drink(self.earth)

		# check if have to move to current task
		if self.task.target_loc!=None and self.task.target_loc!=self.loc and self.task.tid!=1:
			# this means that we already have a goal and another task that requires walking
			if self.goal!=None:
				raise
			self.goal=self.task
			self.task=walk(self.earth.grid,self.loc,self.task.target_loc,self.walk_wait)

		# check if task is complete
		if self.task.complete:
			if self.goal==None:
				self.task=idle(self.earth,self.loc,self.idle_time)
			else:
				self.task=self.goal
				self.goal=None

		# update current task
		self.task.update(self.loc)

		self.loc=tuple(sum(x) for x in zip(self.loc,self.task.move()))

# tasks
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

	def init(self):
		self.name=self.tid_dict[self.tid]
		self.complete=False
		self.dwarf=None

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
		self.tid=0
		self.init()

		self.target_loc=None

		diffs=[(1,0,0),(-1,0,0),(0,1,0),(0,-1,0)]

		self.earth=earth
		self.start=start
		self.loc=start
		self.wait=wait
		self.waited=0
		self.complete=False

		# check if possible to wander or not
		self.stuck=True
		self.movable_diffs=[]
		for diff in diffs:
			if 0<=self.loc[0]+diff[0] and self.loc[0]+diff[0]<self.earth.size[0]:
				if 0<=self.loc[1]+diff[1] and self.loc[1]+diff[1]<self.earth.size[1]:
					if self.earth.earth[self.loc[0]+diff[0]][self.loc[1]+diff[1]][self.loc[2]+diff[2]].empty:
						self.stuck=False
						self.movable_diffs.append(diff)

		# pick a direction at random
		if not self.stuck:
			rng.shuffle(self.movable_diffs)
			for diff in self.movable_diffs:
				if self.earth.earth[self.loc[0]+diff[0]][self.loc[1]+diff[1]][self.loc[2]+diff[2]].empty:
					self.diff=diff
					break

	def update(self,loc):
		pass

	def move(self):
		if not self.stuck:
			self.waited+=1
			if self.waited>self.wait:
				self.waited=0
				if self.loc==self.start:
					self.loc=tuple(sum(x) for x in zip(self.loc,self.diff))
					return self.diff
				else:
					self.loc=tuple(x[0]-x[1] for x in zip(self.loc,self.diff))
					return tuple(-x for x in self.diff)
			else:
				return (0,0,0)
		else:
			return (0,0,0)

class walk(task):
	def __init__(self,grid,loc,target_loc,wait):
		self.tid=1
		self.init()

		self.grid=grid
		self.loc=loc
		self.target_loc=target_loc
		self.wait=wait
		self.waited=0
		self.path=Astar.Astar(grid,self.loc,self.target_loc).get_path()
		print self.path

	def update(self,loc):
		self.loc=loc
		if self.loc==self.target_loc:
			self.complete=True
			print len(self.path)

	def move(self):
		self.waited+=1
		if self.waited>self.wait:
			self.waited=0
			new_loc=self.path.pop()+(self.loc[2],)
			delta=tuple(z[0]-z[1] for z in zip(new_loc,self.loc))
			print delta
			self.loc=new_loc
			return delta
		else:
			return (0,0,0)

class dig(task):
	def __init__(self,earth,loc,wait):
		self.tid=2
		self.init()

		self.earth=earth
		self.target_loc=loc
		self.wait=wait
		self.waited=0

	def update(self,loc):
		self.waited+=1
		if self.waited>=self.wait:
			self.finish()

	def finish(self):
		self.earth.grid[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]]=True
		self.earth.earth[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]].floor=True
		self.complete=True

class mine(task):
	"""
	the location is the top of the hole
	"""
	def __init__(self,earth,loc,wait):
		self.tid=3
		self.init()
		
		self.earth=earth
		self.target_loc=loc
		self.wait=wait
		self.waited=0

	def update(self,loc):
		self.waited+=1
		if self.waited>=self.wait:
			self.finish()

	def finish(self):
		# one beneath is now a floor
		self.earth.grid[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]+1]=True
		self.earth.earth[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]+1].floor=True
		self.earth.earth[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]+1].empty=True
		# itself is now empty, but not a floor
		self.earth.earth[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]].empty=True
		self.earth.earth[self.target_loc[0]][self.target_loc[1]][self.target_loc[2]].floor=False
		self.complete=True

class drink(task):
	def __init__(self,earth):
		self.tid=6
		self.init()
		self.earth=earth

class obj(object):
	"""
	things that are at locations
	0:	pile of earth:	stuff that was dug out of the earth
	1:	axe:			digs in anything
	2:	shovel:			cheaper than axe, digs in soil only
	3:	cart:			hauls one half of one location away
	4:	ladder:			allows for movement vertically
	5:	trading post:	turn resources into moneys
	"""
	oid_dict={
		0:("Pile of earth"),
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

class pile(obj):
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
		diff:			difficulty to dig (0 = nothing, 1 = easy, 10 = quite hard)
		color
		objs:			list of objects contained, must be empty of lid!=0
		empty,floor:	bools. If empty then can walk/climb. If floor then can put stuff on.
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
		self.floor=False
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
		self.grid=np.empty(self.size,dtype=bool)
		for x in range(self.size[0]):
			for y in range(self.size[1]):
				# we note here that z=0 is the surface
				self.earth[x][y][0]=location(0)
				self.earth[x][y][0].empty=True
				self.earth[x][y][0].floor=True
				self.grid[x][y][0]=True
				# put bedrock everywhere else
				for z in range(1,self.size[2]):
					self.earth[x][y][z]=location(1)
					self.grid[x][y][z]=False
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
							if self.earth[tmp_loc[0]][tmp_loc[1]][tmp_loc[2]].lid==1 or True:
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
		self.framerate=40
		self.bg_color=BLACK
		self.screen.fill(self.bg_color)
		self.dwarf_list=[]
		self.task_queue=[]
		self.notification_clock=-1
		self.notification_index=0
		self.log_msgs=[]
		self.paused=False

		pygame.key.set_repeat(200,50)

		self.text("jazzwhiz games",90,WHITE,location=(0,50),centerx=True)
		self.text("presents",45,WHITE,location=(0,150),centerx=True)
		self.text("Dwarf Beards",90,WHITE,location=(0,300),centerx=True)
		self.text("Press enter to begin",30,WHITE,location=(0,500),centerx=True)
		self.text("v %s %s"%(__version__,copyright),15,DARK_GRAY,location=(650,585))
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
		self.screen.fill(self.bg_color)
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

		self.notification_clock=-1
		self.notification_index=4

		self.run()

	def p(self,s):
		if self.notification_clock==-1:
			self.notification_clock=0
		self.log_msgs.append(s)
		print s

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
		self.screen.fill(self.bg_color)

		fs=20
		x_offset1=20
		x_offset2=65

		f=pygame.font.SysFont(None,fs)
		ls=f.get_linesize()

		y=ls
		self.text("esc",20,WHITE,(x_offset1,y))
		self.text("- Exit",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("h",20,WHITE,(x_offset1,y))
		self.text("- Toggle this help screen",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("l",20,WHITE,(x_offset1,y))
		self.text("- Toggle log view",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("space",20,WHITE,(x_offset1,y))
		self.text("- Pause/Unpause",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("n",20,WHITE,(x_offset1,y))
		self.text("- Buy a new dwarf",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("d",20,WHITE,(x_offset1,y))
		self.text("- Dig (horizontal)",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("m",20,WHITE,(x_offset1,y))
		self.text("- Mine (down)",20,WHITE,(x_offset2,y))
		y+=ls
		self.text("<",20,WHITE,(x_offset1,y))
		self.text("- Up one z level",20,WHITE,(x_offset2,y))
		y+=ls
		self.text(">",20,WHITE,(x_offset1,y))
		self.text("- Down one z level",20,WHITE,(x_offset2,y))


		pygame.display.update()
		self.clock.tick(self.framerate)

		helping=True
		while helping:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.unicode=="h":
						helping=False
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
			self.clock.tick(self.framerate)

	def log(self):
		self.screen.fill(self.bg_color)

		y=10
		i=len(self.log_msgs)
		while y<self.screen_size[1]-18 and i>0:
			i-=1
			if i==len(self.log_msgs)-1:
				self.text(self.log_msgs[i],20,YELLOW,(10,y))
			else:
				self.text(self.log_msgs[i],20,WHITE,(10,y))
			y+=18
		
		pygame.display.update()
		self.clock.tick(self.framerate)

		logging=True
		while logging:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.unicode=="l":
						logging=False
				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
			self.clock.tick(self.framerate)

	def text(self,msg,size,color,location=(0,0),centerx=False,centery=False,bold=False):
		f=pygame.font.SysFont(None,size,bold=bold)
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
		self.dwarf_list.append(dwarf(self.earth,loc))
		self.p("Bought dwarf %s"%self.dwarf_list[-1].name)

	def draw(self):
		"""
		todo:	draw all the stuff
		"""
		self.screen.fill(self.bg_color)
		
		# draw the earth
		for x in range(self.earth_size[0]):
			for y in range(self.earth_size[1]):
				if self.earth.earth[x][y][self.level].floor:
					pygame.draw.rect(self.screen,self.earth.earth[x][y][self.level].color,(12*x+2,12*y+2,8,8),3)
				elif self.earth.earth[x][y][self.level].empty:
					continue
				else:
					pygame.draw.rect(self.screen,self.earth.earth[x][y][self.level].color,(12*x+1,12*y+1,10,10),0)
		for task in self.task_queue:
			if task.target_loc==None:
				continue
			if task.target_loc[2]==self.level:
				# digging out
				if task.tid==2:
					pygame.draw.line(self.screen,self.bg_color,
						(12*task.target_loc[0]+10,12*task.target_loc[1]+1),
						(12*task.target_loc[0]+1,12*task.target_loc[1]+10),2)
				# mining down
				if task.tid==3:
					pygame.draw.line(self.screen,self.bg_color,
						(12*task.target_loc[0]+10,12*task.target_loc[1]+1),
						(12*task.target_loc[0]+1,12*task.target_loc[1]+10),2)
					pygame.draw.line(self.screen,self.bg_color,
						(12*task.target_loc[0]+1,12*task.target_loc[1]+1),
						(12*task.target_loc[0]+10,12*task.target_loc[1]+10),2)

		# draw the dwarves
		for dwarf in self.dwarf_list:
			if dwarf.loc[2]==self.level:
				self.text("@",18,RED,(12*dwarf.loc[0],12*dwarf.loc[1]-2),bold=True)
#				pygame.draw.circle(self.screen,self.bg_color,(12*dwarf.loc[0]+6,12*dwarf.loc[1]+6),5,0)

		# draw the focus box
		focus_corners=[
			(12*self.focus[0]-2,12*self.focus[1]-2),
			(12*(self.focus[0]+1),12*self.focus[1]-2),
			(12*(self.focus[0]+1),12*(self.focus[1]+1)),
			(12*self.focus[0]-2,12*(self.focus[1]+1))
			]
		for i in range(4):
			pygame.draw.line(self.screen,WHITE,focus_corners[i],focus_corners[(i+1)%4],2)

		# now we draw RHS of stuff
		self.text("(%i,%i,%i)"%(self.focus[0],self.focus[1],-self.level),15,RED,(610,5))

		if self.focus_loc.floor:
			self.text("%s floor"%self.focus_loc.name,15,RED,(680,5))
		elif self.focus_loc.empty:
			self.text("Air",15,RED,(680,5))
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

		# task Q
		self.text("Task queue: %i"%len(self.task_queue),15,LIGHT_GRAY,(610,520))

		# draw notification
		if self.notification_clock>-1:
			self.text(self.log_msgs[self.notification_index],15,WHITE,(610,535))
			self.notification_clock+=1
			if self.notification_clock==50:
				self.notification_index+=1
				if self.notification_index<len(self.log_msgs):
					self.notification_clock=0
				else:
					self.notification_clock=-1
	
		# is paused?
		if self.paused:
			self.text("<PAUSED>",15,LIGHT_GRAY,(610,550))

	def update(self):
		for dwarf in self.dwarf_list:
			dwarf.update()
		for task in self.task_queue:
			if task.dwarf==None:
				for dwarf in self.dwarf_list:
					if dwarf.task.tid==0:
						if rng.random()<dwarf.laziness:
							dwarf.task=task
							task.dwarf=dwarf
							self.p("Assigned %s to %s"%(task.name,dwarf.name))

	def run(self):
		while True:
			self.events=pygame.event.get()
			for event in self.events:
				if event.type==KEYDOWN:
					# general game action
					if event.key==K_ESCAPE:
						pygame.event.post(pygame.event.Event(QUIT))
					if event.unicode=="h":
						self.help()
					if event.unicode=="l":
						self.log()
					if event.key==K_SPACE:
						self.paused=(not self.paused)

					# dwarf action
					if event.unicode=="n":
						if self.focus_loc.floor:
							self.buy_dwarf(self.focus+(self.level,))
						else:
							self.p("Cannot place dwarf here.")

					# adding tasks
					if event.unicode=="m":
						if self.level>=self.earth_size[2]-1:
							self.p("If you mine any deeper you'll hit magma, better not...")
						elif self.earth.earth[self.focus[0]][self.focus[1]][self.level+1].empty:
							self.p("It is already empty beneath here.")
						else:
							something=False
							for task in self.task_queue:
								if task.tid in [2,3]:
									if task.target_loc==self.focus+(self.level,):
										self.p("We are already %s here."%task.__str__().lower())
										something=True
							if not something:
								self.task_queue.append(mine(self.earth,self.focus+(self.level,),
									self.earth.earth[self.focus[0]][self.focus[1]][self.level+1].diff))
					if event.unicode=="d":
						if self.focus_loc.empty:
							self.p("It is already empty here.")
						else:
							something=False
							for task in self.task_queue:
								if task.tid in [2,3]:
									if task.target_loc==self.focus+(self.level,):
										self.p("We are already %s here."%task.__str__().lower())
										something=True
							if not something:
								self.task_queue.append(dig(self.earth,self.focus+(self.level,),self.focus_loc.diff))

					# move focus in the z direction
					if event.unicode=="<":
#						self.p("Moving out of the earth one level")
						self.level=max(self.level-1,0)
					if event.unicode==">":
#						self.p("Moving into the earth one level")
						self.level=min(self.level+1,self.earth_size[2]-1)

					# move focus in the plane, modularly (I'm not a monster)
					mod=1
					if pygame.key.get_mods() & KMOD_SHIFT:
						mod=10
					if event.key==K_UP:
						self.focus=(self.focus[0],(self.focus[1]-mod)%self.earth_size[1])
					if event.key==K_DOWN:
						self.focus=(self.focus[0],(self.focus[1]+mod)%self.earth_size[1])
					if event.key==K_LEFT:
						self.focus=((self.focus[0]-mod)%self.earth_size[0],self.focus[1])
					if event.key==K_RIGHT:
						self.focus=((self.focus[0]+mod)%self.earth_size[0],self.focus[1])

					self.focus_loc=self.earth.earth[self.focus[0]][self.focus[1]][self.level]

				if event.type==pygame.QUIT:
					pygame.quit()
					sys.exit()
			if not self.paused:
				self.update()

			self.draw()
			pygame.display.update()
			self.clock.tick(self.framerate)


