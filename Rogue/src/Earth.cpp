#include <vector>
#include <algorithm>

#include <iostream>

#include "Earth.h"
#include "Location.h"

#include "Progress.h"

Earth::Earth()
{
	allocated = false;
}

Earth::Earth(int max_x, int max_y)
{
	allocated = false;
	earth_size[0] = max_x;
	earth_size[1] = max_y;
	locations = new Location*[earth_size[0]];
	for (int x = 0; x < earth_size[0]; x++)
		locations[x] = new Location[earth_size[1]];
	allocated = true;

	Progress_Bar pbar;
	int history_size = 10000;
	for (int i = 0; i < history_size; i++) // how much initial history
	{
		update();
		pbar.update((double)i / history_size);
	}
}

Earth::~Earth()
{
	if (allocated)
	{
		for (int x = 0; x < earth_size[0]; x++)
			delete [] locations[x];
		delete [] locations;
		allocated = false;
	}
}

void Earth::update()
{
	for (int x = 0; x < earth_size[0]; x++)
	{
		for (int y = 0; y < earth_size[1]; y++)
		{
			locations[x][y].update();
		} // y
	} // x
}

void Earth::live_monster_data()
{
	// collect data
	std::vector<std::vector<int>> live_counts(Monster_Bases.size());
	for (int x = 0; x < earth_size[0]; x++)
	{
		for (int y = 0; y < earth_size[1]; y++)
		{
			for (uint i = 0; i < locations[x][y].monsters.size(); i++)
			{
				while (locations[x][y].monsters[i].lvl >= (int)live_counts[locations[x][y].monsters[i].index].size())
					live_counts[locations[x][y].monsters[i].index].push_back(0);
				live_counts[locations[x][y].monsters[i].index][locations[x][y].monsters[i].lvl]++;
			}
		} // y
	} // x

	// print data
	std::cout << "Live monsters" << std::endl;
	int total;
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		total = 0;
		std::cout << "  " << Monster_Bases[i].name << std::endl;
		for (uint j = 0; j < live_counts[i].size(); j++)
		{
			std::cout << "    " << j << " " << live_counts[i][j] << std::endl;
			total += live_counts[i][j];
		}
		std::cout << "    " << total << std::endl;
	}
}

