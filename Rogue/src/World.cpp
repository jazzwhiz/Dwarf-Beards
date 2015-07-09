#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

#include "World.h"
#include "Dwarf.h"
#include "Draw.h"
#include "Monster.h"
#include "Earth.h"
#include "Location.h"
#include "rng.h"

World::World()
{
	version = "0.02";
	copyright = "(c) 2015 Peter Denton";
	data_dir = "../data/";
	draw::init(this);

	if (not draw::title(this))
		return;

	turn = 1;

//	rng.seed(1888);
	init_names();
	player = Dwarf(random_name());
	std::cout << "Welcome, " << player.name << std::endl;

	read_monsters(this);
	read_locations(this);

	std::cout << "Generating earth's history..." << std::endl;
	earth = new Earth(21, 21);
	std::cout << "History generated." << std::endl;

	location[0] = earth->earth_size[0] / 2;
	location[1] = earth->earth_size[1] / 2;

	earth->locations[location[0]][location[1]].fog = false;

	run();
}

void World::run()
{
	if (not draw::dwarf_profile(this))
	{
		quit();
		return;
	}

	int r;
	bool playing = true;
	while (playing)
	{
		r = draw::earth(this);
		switch (r)
		{
			case 0:
				playing = false;
				quit();
				return;
			case 1: // goto char screen
				break;
			case 2: // left
				location[0]--;
				location[0] = std::max(location[0], 0);
				break;
			case 3: // right
				location[0]++;
				location[0] = std::min(location[0], earth->earth_size[0] - 1);
				break;
			case 4: // up
				location[1]--;
				location[1] = std::max(location[1], 0);
				break;
			case 5: // down
				location[1]++;
				location[1] = std::min(location[1], earth->earth_size[1] - 1);
				break;
		}
	}

		quit();
}

void World::init_names()
{

	std::string tmp, tmp2;

	std::ifstream firstfile(data_dir + "Names/first.txt");
	if (firstfile.is_open())
	{
		while (firstfile)
		{
			getline(firstfile, tmp);
			if (tmp == "") continue;
			tmp2 = strdup(tmp.c_str());
			firsts.push_back(tmp2);
		}
	}
	firstfile.close();

	std::ifstream lastfile(data_dir + "Names/last.txt");
	if (lastfile.is_open())
	{
		while (lastfile)
		{
			getline(lastfile, tmp);
			if (tmp == "") continue;
			tmp2 = strdup(tmp.c_str());
			lasts.push_back(tmp2);
		}
	}
	lastfile.close();
}

std::string World::random_name()
{
	return firsts[rng.rand_int(firsts.size() - 1)] + " " + lasts[rng.rand_int(lasts.size() - 1)];
}

World::~World()
{
	std::cout << "Deleting world..." << std::endl;
	draw::clean_up();
}

void World::quit()
{
	std::cout << "Quitting..." << std::endl;

	// output data
//	earth->live_monster_data();
//	dead_monster_data();

	delete earth;
}

