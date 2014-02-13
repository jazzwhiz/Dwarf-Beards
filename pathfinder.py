"""
(c) 2014 Peter Denton
"""
#!/usr/bin/python

def jump(earth,cx,cy,dx,dy,start,end):
	"""
	cx,cy=current
	dx,dy=direction going
	start,end are tuples
	returns a tuple
	"""
	nextx=cx+dx
	nexty=cy+dy

	# blocked case
	if not earth[nextx][nexty].empty:
		return None

	# if node candidate is goal then we are done	
	if nextx==end[0] and nexty==end[1]:
		return end

	
	if dx!=0 and dy!=0:
		


def idSuccessor(current,start,end):
	return

def path(earth,start,end):
	"""
	earth is a 3d array of elements that have the property "empty", if empty it is walkable, else else
	start,end are 3-tuples
	returns a list of 3-tuples with the first tuple one step away from start and the last is equal to the end
	"""
	return
