#ifndef Monster_H
#define Monster_H

#include <string>
#include <vector>

class World;

class Monster_Base
{
	public:
		Monster_Base() {};
		Monster_Base(std::string name, int base_stats[15]);
		std::string name;
		int base_stats[15];
		bool can_be_lvl(int lvl);
};

class Monster
{
	public:
		Monster(int lvl);
		~Monster();
		std::string name;
		int lvl, exp, base_stats[15], stats[6]; // maxhp, atk, matk, def, mdef, exp (awarded)
		double hp;
		bool damage(double damage); // receive damage, bool is if it is still alive
		void gain_exp(int _exp);
		void heal();
};

void read_monsters(World* w);
void read_monster(World* w, std::string name);

#endif
