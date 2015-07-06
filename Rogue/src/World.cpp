#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

#include "World.h"
#include "Dwarf.h"
#include "Draw.h"
#include "Monster.h"
#include "rng.h"

World::World()
{
	version = "0.01";
	copyright = "(c) 2015 Peter Denton";

	rng.seed(1888);
	data_dir = "../data/";
	init_names();
	player = Dwarf(random_name());
	std::cout << "Welcome, " << player.name << std::endl;

	draw::init(*this);
	read_monsters(*this);
	run();
}

void World::run()
{
	draw::title(*this);
}

void World::init_names()
{
	std::ifstream firstfile(data_dir + "Names/first.txt");
	std::ifstream lastfile(data_dir + "Names/last.txt");

	std::string tmp, tmp2;

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
	firstfile.close();
	lastfile.close();
}

std::string World::random_name()
{
	return firsts[rng.rand_int(firsts.size() - 1)] + " " + lasts[rng.rand_int(lasts.size() - 1)];
}

void World::quit()
{
	std::cout << "Quitting..." << std::endl;
	draw::clean_up();
}
