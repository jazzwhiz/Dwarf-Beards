#ifndef ASTAR_H
#define ASTAR_H

#include <vector>

class Astar
{
	private:
		std::vector<int[3]> closedset;
		std::vector<int[3]> openset;
	public:
		Astar(int grid, int start[3], int end[3]);
		
};

#endif
