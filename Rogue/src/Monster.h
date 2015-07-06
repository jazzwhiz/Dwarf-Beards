#ifndef Monster_H
#define Monster_H

#include <string>
#include <vector>

#include "World.h"

class Monster_Base
{
	public:
		Monster_Base() {};
		Monster_Base(std::string name, int stats[15]);
		std::string name;
		int stats[15];

		bool can_be_lvl(int lvl);
};

class Monster
{
	public:
		Monster(int lvl);
		std::string name;
		int lvl, stats[6];
};

void read_monsters(World w);
void read_monster(World w, std::string file_name);

#endif
