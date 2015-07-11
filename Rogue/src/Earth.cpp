#include <vector>
#include <algorithm>

#include <iostream>

#include "Earth.h"
#include "Location.h"
#include "Camp.h"

#include "Progress.h"
#include "rng.h"

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

	// starting location is a camp
	locations[max_x / 2][max_y / 2].evil = 0;
	locations[max_x / 2][max_y / 2].camp = Camp();

	// place some other camps
	int x, y;
	for (int i = 0; i < 4; i++)
	{
		x = rng.rand_int(earth_size[0] - 1);
		y = rng.rand_int(earth_size[1] - 1);

		locations[x][y].evil = 0;
		locations[x][y].camp = Camp();
	}

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
	// todo: spread evil (and weather)

	int evil_sum, n_neighbors;
	double difference;

	for (int x = 0; x < earth_size[0]; x++)
	{
		for (int y = 0; y < earth_size[1]; y++)
		{
			locations[x][y].update();

			// move evil
			if (locations[x][y].camp.index != 0 or rng.rand() > 0.02)
				continue;

			// sometimes randomly change
			if (rng.rand() < 0.3)
			{
				// slight preference for evil to combat the fixed serene camps
				if (rng.rand() < 0.505)
					locations[x][y].evil++;
				else
					locations[x][y].evil--;
				locations[x][y].evil = std::max(locations[x][y].evil, 0);
				locations[x][y].evil = std::min(locations[x][y].evil, 10);
			}

			evil_sum = 0;
			n_neighbors = 0;
			if (x > 0)
			{
				evil_sum += locations[x - 1][y].evil;
				n_neighbors++;
			}
			if (x < earth_size[0] - 1)
			{
				evil_sum += locations[x + 1][y].evil;
				n_neighbors++;
			}
			if (y > 0)
			{
				evil_sum += locations[x][y - 1].evil;
				n_neighbors++;
			}
			if (y < earth_size[1] - 1)
			{
				evil_sum += locations[x][y + 1].evil;
				n_neighbors++;
			}

			difference = (double)evil_sum / n_neighbors - locations[x][y].evil;
			if (rng.rand(10) < std::abs(difference))
				locations[x][y].evil += (difference > 0 ? 1 : -1);

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

