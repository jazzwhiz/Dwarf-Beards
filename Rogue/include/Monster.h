#ifndef Monster_H
#define Monster_H

#include <string>
#include <vector>

#include "Being.h"

class World;

class Monster_Base
{
	public:
		Monster_Base() {};
		Monster_Base(std::string name, int base_stats[15]);
		std::string name;
		int base_stats[15];
		bool can_be_lvl(int lvl);
		std::vector<int> deaths; // deaths[i] is the number of i lvls that died
};

class Monster : public Being
{
	public:
		Monster(int lvl);
		~Monster();
		std::string name;
		int lvl, exp, index,base_stats[15];//, stats[6]; // maxhp, atk, matk, def, mdef, exp (awarded)

		void gain_exp(int _exp);
		void heal(double _hp);
};

extern std::vector<Monster_Base> Monster_Bases;

void read_monsters(World* w);
void read_monster(World* w, std::string name);
void dead_monster_data();

#endif
