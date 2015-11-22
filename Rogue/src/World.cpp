#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

#include "World.h"
#include "Dwarf.h"
#include "Draw.h"
#include "Keyboard.h"
#include "Monster.h"
#include "Earth.h"
#include "Location.h"
#include "Camp.h"
#include "Battle.h"
#include "Modification.h"

#include "rng.h"

World::World()
{
	version = "0.04";
	copyright = "(c) 2015 Peter Denton";
	data_dir = "../data/";
	draw::init(this);

	if (draw::title(this) == 0)
		return;

	turn = 1;

//	rng.seed(1888);
	init_names();
	player = Dwarf(random_name());
	std::cout << "Welcome, " << player.name << std::endl;

	read_monsters(this);
	read_locations(this);

	camp_init();

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
	int status = 1;
	bool playing = true;

	while (playing)
	{
		switch (status)
		{
			case 0: // quit
				playing = false;
				break;
			case 1: // dwarf profile
				status = draw::dwarf_profile(this);
				break;
			case 2: // main screen
				status = draw::earth(this);
				break;
			case 3: // left
			case 4: // right
			case 5: // up
			case 6: // down
				move(status);
				status = 2; // go to main screen
				turn++;
				break;
			case 7: // wait
				wait_turn();
				status = 2;
				turn++;
				break;
			case 8: // battle
				status = dwarf_battle(this);
				turn++;
				break;
			case 9: // tavern
				status = enter_building("Tavern");
				break;
			case 10: // inn
				status = enter_building("Inn");
				break;
			case 11: // help
				draw::help();
				status = 2;
				break;
			default:
				break;
		}
	}

	quit();
}

void World::move(int direction)
{
	// check if going off map
	bool off_map = false;
	switch (direction)
	{
		case 3:
			off_map = location[0] == 0;
			break;
		case 4:
			off_map = location[0] == earth->earth_size[0] - 1;
			break;
		case 5:
			off_map = location[1] == 0;
			break;
		case 6:
			off_map = location[1] == earth->earth_size[1] - 1;
			break;
	}
	if (off_map)
		return;

	// if asleep, wait instead of move
	if (player.sleepiness >= 1)
	{
		wait_turn();
		return;
	}

	// move location
	switch (direction)
	{
		case 3: // left
			location[0]--;
			break;
		case 4: // right
			location[0]++;
			break;
		case 5: // up
			location[1]--;
			break;
		case 6: // down
			location[1]++;
			break;
	}

	earth->locations[location[0]][location[1]].fog = false;

	// update sleepiness, and off screen monsters
	player.sleepiness += earth->locations[location[0]][location[1]].diff / 100.;
	player.sleepiness = std::min(player.sleepiness, 1.);
	update();
}

void World::wait_turn()
{
	player.sleepiness -= 0.05;
	player.sleepiness = std::max(player.sleepiness, 0.);
	update();
}

int World::enter_building(std::string building_name)
{
	for (unsigned i = 0; i < earth->locations[location[0]][location[1]].camp.buildings.size(); i++)
	{
		if (building_name == earth->locations[location[0]][location[1]].camp.buildings[i]->name)
		{
			draw::inside_building(this, earth->locations[location[0]][location[1]].camp.buildings[i]);
			int ret = keyboard::building(*earth->locations[location[0]][location[1]].camp.buildings[i]);
			if (ret > 2)
			{
				earth->locations[location[0]][location[1]].camp.buildings[i]->modifications[ret - 2]->modify(&player);
				ret = 2; // go to main
			}
			if (ret == 1)
				ret = 2;
			return ret;
			// todo actually buy stuff
		}
	}
	return 2;
}

void World::update()
{
	earth->update();
	player.update();
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
	dead_monster_data();

	delete earth;
}

