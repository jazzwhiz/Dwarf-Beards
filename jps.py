"""
(c) 2014 Peter Denton
My implementation of algorithm found
http://users.cecs.anu.edu.au/~dharabor/data/papers/harabor-grastien-aaai11.pdf
"""
#!/usr/bin/python

__version__="0.01"

# done
diffs=[]
for x in [-1,0,1]:
	for y in [-1,0,1]:
		diffs.append((x,y))
diffs.remove((0,0))

# done
def dot(a,b):
	return tuple(z[0]*z[1] for z in zip(a,b))

# done
def orthogonal(a,b):
	return dot(a,b)==0

# done
def manhattan(dx,dy):
	return dx+dy

# done
def jump(grid,current,d,start,end,z):
	neighbor=step(current,d)
	if not in_grid(grid,neighbor,z):
		return None
	if not walkable(grid,neighbor,z):
		return None
	if neighbor==end:
		return neighbor
	if is_forced(grid,neighbor,z):
		return neighbor
	if is_diagonal(d):
		for i in range(2):
			di=[0,0]
			di[i]=d[i]
			if jump(grid,neighbor,di,start,end,z) is not None:
				return neighbor
	return jump(grid,neighbor,d,start,end,z)

# done
def is_forced(grid,x,z):
	ns=neighbors(grid,x,z)
	for n in ns:
		if not in_grid(grid,n,z):
			return True
		if not walkable(grid,n,z):
			return True
	return False

# done (only really defined for 2d)
def is_diagonal(d):
	return d.count(0)==0

# done
def neighbors(grid,x,z):
	ns=[step(diff,x) for diff in diffs]
	for neighbor in ns:
		if not in_grid(grid,neighbor,z) or not walkable(grid,neighbor,z):
			ns.remove(neighbor)
	return ns

# maybe done?
def prune(grid,current,d):
	ns=[step(current,d)]

	for diff in diffs:
		if orthogonal(d,diff):
			if not walkable(step(current,diff)):
				ns.append(step(current,diff))
	return ns

# done
def id_successors(grid,current,start,end,z):
	successors=[]

	d=direction(current,end)
	# d -> (1,-1,0) or whatever
	d=tuple(a/max(1,abs(a)) for a in d)

	ns=prune(grid,current,d)
	for neighbor in ns:
		neighbor=jump(grid,current,d,start,end,z)
		successors.append(neighbor)
	return successors

# returns the vector b-a, that is, from a to b
def direction(a,b):
	return tuple(z[1]-z[0] for z in zip(a,b))

# adds the vecotrs
def step(a,b):
	return tuple(sum(z) for z in zip(a,b))

# returns the bool, basically to deal with dumb indices
def walkable(grid,x,z):
	return grid[x[0]][x[1]][z]

# determines if we are within the grid
def in_grid(grid,x,z):
	for a in x:
		if a<0:
			return False
	s=grid.shape
	for i in range(2):
		if x[i]>=s[i]:
			return False
	return True

def path(grid,start,end):
	"""
	grid is a 3d array of booleans that answer: Walkable?
	start,end are 3-tuples
	returns a list of 3-tuple diffs
	"""
	print start,end
	assert start[2]==end[2],"only doing 2d atm"
	z=start[2]
	start=start[:2]
	end=end[:2]
	successors=id_successors(grid,start,start,end,z)
	print successors
	assert len(successors)==1
	return direction(start,successors[0])+(z,)



