"""
(c) 2014 Peter Denton
My implementation of algorithm found
http://www.laurentluce.com/posts/solving-mazes-using-python-simple-recursivity-and-a-search/
"""
#!/usr/bin/python

import heapq

__version__="0.02"

class Cell(object):
	def __init__(self,x,y,reachable):
		self.reachable = reachable
		self.x = x
		self.y = y
		self.parent = None
		self.g = 0
		self.h = 0
		self.f = 0
	
class Astar(object):
	def __init__(self,grid,start,end):
		assert start[2]==end[2],"only doing 2d atm"
		start=start[:2]
		end=end[:2]
		self.grid=grid[:,:,0] # only the top shelf for now
		self.start=start
		self.end=end

		self.op = []
		heapq.heapify(self.op)
		self.cl = set()
		self.cells = []

		self.init_grid(start,end)
		self.path=[]

	def init_grid(self,start,end):
		self.gridWidth,self.gridHeight=self.grid.shape
		for x in range(self.gridWidth):
			for y in range(self.gridHeight):
				reachable = self.grid[x,y]
				self.cells.append(Cell(x, y, reachable))
		self.start = self.get_cell(start[0],start[1])
		self.end = self.get_cell(end[0],end[1])

	def get_heuristic(self, cell):
		return 10 * (abs(cell.x - self.end.x) + abs(cell.y - self.end.y))

	def get_cell(self, x, y):
		return self.cells[x * self.gridHeight + y]

	def get_adjacent_cells(self, cell):
		cells = []
		if cell.x < self.gridWidth-1:
			cells.append(self.get_cell(cell.x+1, cell.y))
		if cell.y > 0:
			cells.append(self.get_cell(cell.x, cell.y-1))
		if cell.x > 0:
			cells.append(self.get_cell(cell.x-1, cell.y))
		if cell.y < self.gridHeight-1:
			cells.append(self.get_cell(cell.x, cell.y+1))
		return cells

	def display_path(self):
		cell = self.end
		while cell.parent is not self.start:
			cell = cell.parent
			self.path.append((cell.x,cell.y))
#			print 'path: cell: %d,%d' % (cell.x, cell.y)

	def update_cell(self, adj, cell):
		adj.g = cell.g + 10
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
						if c.g > cell.g + 10:
							self.update_cell(c, cell)
					else:
						self.update_cell(c, cell)
						# add adj cell to open list
						heapq.heappush(self.op, (c.f, c))

	def get_path(self):
		self.process()
		return self.path
