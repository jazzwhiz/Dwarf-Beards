#include <vector>
#include <algorithm>
#include <fstream>

#include <iostream>

#include "Earth.h"
#include "Location.h"
#include "Camp.h"

#include "Draw.h"

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

	Progress_Bar pbar;
	int history_size = 10000;
	for (int i = 0; i < history_size; i++) // how much initial history
	{
		update_global();
		pbar.update((double)i / history_size);
		draw::progress_bar((double)i / history_size, draw::screen_size[0] / 2, 450, 20, 1);
	}
	for (int i = 0; i < history_size; i++) // how much initial history
	{
		update_local();
		pbar.update((double)i / history_size);
		draw::progress_bar((double)i / history_size, draw::screen_size[0] / 2, 450, 20, 1);
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
	update_global();
	update_local();
}

void Earth::update_global()
{
	// todo: spread weather
	// todo: in world gen, update evil first, then place monsters

	int evil_sum, n_neighbors;
	double temperature_sum, precipitation_sum;
	double difference;

	for (int x = 0; x < earth_size[0]; x++)
	{
		for (int y = 0; y < earth_size[1]; y++)
		{
			// move evil
			if (locations[x][y].camp.index != 0 or rng.rand() > 0.02)
				continue;

			// sometimes randomly change
			if (rng.rand() < 0.2)
			{
				// slight preference for evil to combat the fixed serene camps
				if (rng.rand() < 0.505)
					locations[x][y].evil++;
				else
					locations[x][y].evil--;
				locations[x][y].evil = std::max(locations[x][y].evil, 0);
				locations[x][y].evil = std::min(locations[x][y].evil, 10);
			}

			// check neighbors
			evil_sum = 0;
			temperature_sum = 0;
			precipitation_sum = 0;
			n_neighbors = 0;
			if (x > 0)
			{
				evil_sum += locations[x - 1][y].evil;
				temperature_sum += locations[x - 1][y].weather.temperature;
				precipitation_sum += locations[x - 1][y].weather.temperature;
				n_neighbors++;
			}
			if (x < earth_size[0] - 1)
			{
				evil_sum += locations[x + 1][y].evil;
				temperature_sum += locations[x + 1][y].weather.temperature;
				precipitation_sum += locations[x + 1][y].weather.temperature;
				n_neighbors++;
			}
			if (y > 0)
			{
				evil_sum += locations[x][y - 1].evil;
				temperature_sum += locations[x][y - 1].weather.temperature;
				precipitation_sum += locations[x][y - 1].weather.temperature;
				n_neighbors++;
			}
			if (y < earth_size[1] - 1)
			{
				evil_sum += locations[x][y + 1].evil;
				temperature_sum += locations[x][y + 1].weather.temperature;
				precipitation_sum += locations[x][y + 1].weather.temperature;
				n_neighbors++;
			}

			difference = (double)evil_sum / n_neighbors - locations[x][y].evil;
			if (rng.rand(10) < std::abs(difference))
				locations[x][y].evil += (difference > 0 ? 1 : -1);

			difference = temperature_sum / n_neighbors - locations[x][y].weather.temperature;
			if (rng.rand(5) < std::abs(difference))
				locations[x][y].weather.temperature += (difference > 0 ? 1 : -1) * rng.rand(3);

			difference = precipitation_sum / n_neighbors - locations[x][y].weather.precipitation;
			if (rng.rand(4) < std::abs(difference))
				locations[x][y].weather.precipitation += (difference > 0 ? 1 : -1);

			// TODO weather fronts

		} // y
	} // x
}

void Earth::update_local()
{
	for (int x = 0; x < earth_size[0]; x++)
	{
		for (int y = 0; y < earth_size[1]; y++)
		{
			locations[x][y].update();
		}
	}
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
	std::cout << "Writing live monster data to file..." << std::endl;
	std::ofstream lmd("logs/live_monster_data.txt");

	lmd << "---------------" << std::endl;
	lmd << " Live Monsters" << std::endl;
	lmd << "---------------" << std::endl;

	int total;
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		total = 0;
		lmd << "  " << Monster_Bases[i].name << std::endl;
		for (uint j = 0; j < live_counts[i].size(); j++)
		{
			lmd << "    " << j << " " << live_counts[i][j] << std::endl;
			total += live_counts[i][j];
		}
		lmd << "    " << total << std::endl;
	}
}

