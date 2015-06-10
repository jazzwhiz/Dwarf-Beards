"""
(c) 2014 Peter Denton
My implementation of algorithm found
http://www.laurentluce.com/posts/solving-mazes-using-python-simple-recursivity-and-a-search/
"""
#!/usr/bin/python

import heapq
#from world import rng

# todo: make it select horiz/vert first randomly
__version__="0.10"

class Cell(object):
	def __init__(self,x,y,z,reachable):
		self.reachable = reachable
		self.x = x
		self.y = y
		self.z = z
		self.parent = None
		self.g = 0
		self.h = 0
		self.f = 0
	
class Astar(object):
	def __init__(self,grid,rng,start,end):
		self.grid=grid
		self.rng=rng
		self.start=start
		self.end=end

		self.op = []
		heapq.heapify(self.op)
		self.cl = set()
		self.cells = {}

		self.init_grid()
		self.path=[]

	def init_grid(self):
		self.gridWidth,self.gridHeight,self.gridDepth=self.grid.shape
		for x in range(self.gridWidth):
			for y in range(self.gridHeight):
				for z in range(self.gridDepth):
					reachable = self.grid[x,y,z]
					self.cells[(x,y,z)]=Cell(x, y, z,reachable)
		# this is necessary for digging in an uncreachable spot
		self.cells[self.end].reachable=True
		self.start = self.get_cell(*self.start)
		self.end = self.get_cell(*self.end)

	def get_heuristic(self, cell):
		return abs(cell.x - self.end.x) + abs(cell.y - self.end.y)+abs(cell.z-self.end.z)

	def get_cell(self, x, y,z):
		return self.cells[(x,y,z)]

	def get_adjacent_cells(self, cell):
		cells = []
		if cell.x < self.gridWidth-1:
			cells.append(self.get_cell(cell.x+1, cell.y,cell.z))
		if cell.x > 0:
			cells.append(self.get_cell(cell.x-1, cell.y,cell.z))
		if cell.y < self.gridHeight-1:
			cells.append(self.get_cell(cell.x, cell.y+1,cell.z))
		if cell.y > 0:
			cells.append(self.get_cell(cell.x, cell.y-1,cell.z))
		if cell.z < self.gridDepth-1:
			cells.append(self.get_cell(cell.x, cell.y,cell.z+1))
		if cell.z > 0:
			cells.append(self.get_cell(cell.x, cell.y,cell.z-1))
		self.rng.shuffle(cells)
		return cells

	def display_path(self):
		cell = self.end
		while cell.parent is not self.start:
			cell = cell.parent
			self.path.append((cell.x,cell.y,cell.z))

	def update_cell(self, adj, cell):
		adj.g = cell.g + 1
		adj.h = self.get_heuristic(adj)
		adj.parent = cell
		adj.f = adj.h + adj.g

	def process(self):
		# add starting cell to open heap queue
		heapq.heappush(self.op, (self.start.f, self.start))
		while len(self.op):
			# pop cell from heap queue 
			f, cell = heapq.heappop(self.op)
			# add cell to closed list so we don't process it twice
			self.cl.add(cell)
			# if ending cell, display found path
			if cell is self.end:
				self.display_path()
				break
			# get adjacent cells for cell
			adj_cells = self.get_adjacent_cells(cell)
			for c in adj_cells:
				if c.reachable and c not in self.cl:
					if (c.f, c) in self.op:
						# if adj cell in open list, check if current path is
						# better than the one previously found for this adj
						# cell.
						if c.g > cell.g + 1:
							self.update_cell(c, cell)
					else:
						self.update_cell(c, cell)
						# add adj cell to open list
						heapq.heappush(self.op, (c.f, c))

	def get_path(self):
		if self.start==self.end:
			return []
		self.process()
		return self.path
