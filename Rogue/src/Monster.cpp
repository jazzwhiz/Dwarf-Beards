#include <string>
#include <fstream>
#include <cstring>
#include <assert.h>
#include <vector>
#include <algorithm>

#include <iostream>

#include "Monster.h"
#include "World.h"

#include "rng.h"

std::vector<Monster_Base> Monster_Bases;

Monster_Base::Monster_Base(std::string name, int _base_stats[15])
: name(name)
{
	for (int i = 0; i < 15; i++)
		base_stats[i] = _base_stats[i];
}

bool Monster_Base::can_be_lvl(int lvl)
{
	return (lvl >= base_stats[12] and (lvl <= base_stats[13] or base_stats[13] == -1));
}

Monster::Monster(int lvl)
: lvl(lvl), exp(0)
{
	int rarity_sum = 0;
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		if (Monster_Bases[i].can_be_lvl(lvl))
			rarity_sum += Monster_Bases[i].base_stats[14];
	}

	Monster_Base mb;
	int rarity_rng = rng.rand_int(1, rarity_sum);
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		if (Monster_Bases[i].can_be_lvl(lvl))
		{
			if (rarity_rng <= Monster_Bases[i].base_stats[14])
			{
				mb = Monster_Bases[i];
				break;
			}
			rarity_rng -= Monster_Bases[i].base_stats[14];
		}
	}

	name = mb.name;
	for (int i = 0; i < 15; i++)
		base_stats[i] = mb.base_stats[i];
	for (int i = 0; i < 6; i++)
		stats[i] = base_stats[2 * i] + lvl * base_stats[2 * i + 1];
	hp = stats[0];
}

Monster::~Monster()
{
//	std::cout << name << " was killed." << std::endl;
}

bool Monster::damage(double damage)
{
	hp -= damage;
//	std::cout << name << " took " << damage << " damage has " << hp << "/" << stats[0] << " hp" << std::endl;
	return (hp > 0);
}

void Monster::gain_exp(int _exp)
{
	std::cout << name << "(" << lvl << ") gains " << _exp << " exp" << std::endl;
	exp += _exp;
	while (exp >= 50 * lvl * lvl)
	{
		exp -= 50 * lvl * lvl;
		lvl++;
		stats[0] += 5;
		for (int i = 1; i < 6; i++)
			stats[i] += base_stats[i * 2 + 1];
	}
}

void Monster::heal()
{
	hp += 2;
	if (hp > stats[0])
		hp = stats[0];
}

void read_monsters(World* w)
{
	std::string tmp, tmp2;
	std::ifstream names(w->data_dir + "Monsters/Names");
	assert (names.is_open());

	while (names)
	{
		getline(names, tmp);
		if (tmp == "") continue;
		tmp2 = strdup(tmp.c_str());
		read_monster(w, tmp2);
	}
	names.close();
}

void read_monster(World* w, std::string name)
{
	std::ifstream monster(w->data_dir + "Monsters/" + name);
	assert (monster.is_open());

	int base_stats[15];

	int i = 0;
	while (monster and i < 15)
	{
		monster >> base_stats[i];
		i++;
	}
	monster.close();

	Monster_Base mb(name, base_stats);
	Monster_Bases.push_back(mb);
}

