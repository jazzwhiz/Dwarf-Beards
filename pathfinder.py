"""
(c) 2014 Peter Denton
My implementation of algorithm found
http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf
"""
#!/usr/bin/python

# done
def manhattan(dx,dy):
	return dx+dy

# done
def jump(grid,current,d,start,end):
	neighbor=step(current,d)
	if not in_grid(grid,neighbor):
		return None
	if not walkable(grid,neighbor):
		return None
	if neighbor==end:
		return neighbor
	for neighborp in neighbors(neighbor):
		if is_forced(grid,neighborp):
			return neighbor
	if is_diagonal(d):
		for i in range(3):
			di=[0,0,0]
			di[i]=d[i]
			if jump(grid,neighbor,di,start,end) is not None:
				return neighbor
	return jump(grid,neighbor,start,end)

# not done
def is_forced(grid,x):
	

# done
diffs=[]
for x in [-1,0,1]:
	for y in [-1,0,1]:
		for z in [-1,0,1]:
			diffs.append((x,y,z))
diffs.remove((0,0,0))

# done
def neighbors(grid,x):
	ns=[step(diff,x) for diff in diffs]
	for neighbor in ns:
		if not in_grid(grid,neighbor):
			ns.remove(neighbor)
	return ns

# not done
def prune(grid,x,ns,d):
	if 

# done
def id_successor(grid,current,start,end):
	successors=[]
	d=direction(current,neighbor)
	ns=prune(grid,current,neighbors(grid,current),d)
	for neighbor in ns:
		neighbor=jump(grid,current,d,start,end)
		successors.append(neighbor)
	return successors

# returns the vector b-a, that is, from a to b
def direction(a,b):
	return tuple(z[1]-z[0] for z in zip(a,b))

# adds the vecotrs
def step(x,d):
	return tuple(z[0]+z[1] for z in zip(a,b))

# returns the bool, basically to deal with dumb indices
def walkable(grid,x):
	return grid[x[0]][x[1]][x[2]]

# determines if we are within the grid
def in_grid(grid,x):
	for a in x:
		if a<0:
			return False
	s=grid.shape
	for i in range(3):
		if x[i]>=s[i]: return False
	return True

def path(grid,start,end):
	"""
	grid is a 3d array of booleans that answer: Walkable?
	start,end are 3-tuples
	returns a list of 3-tuple diffs
	"""
	return
