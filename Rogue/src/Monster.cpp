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

Monster_Base::Monster_Base(std::string name, int _stats[15])
: name(name)
{
	for (int i = 0; i < 15; i++)
		stats[i] = _stats[i];
}

bool Monster_Base::can_be_lvl(int lvl)
{
	return (lvl >= stats[12] and (lvl <= stats[13] or stats[13] == -1));
}

Monster::Monster(int lvl)
: lvl(lvl), exp(0)
{
	int rarity_sum = 0;
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		if (Monster_Bases[i].can_be_lvl(lvl))
			rarity_sum += Monster_Bases[i].stats[14];
	}

	Monster_Base mb;
	int rarity_rng = rng.rand_int(1, rarity_sum);
	for (uint i = 0; i < Monster_Bases.size(); i++)
	{
		if (Monster_Bases[i].can_be_lvl(lvl))
		{
			if (rarity_rng <= Monster_Bases[i].stats[14])
			{
				mb = Monster_Bases[i];
				break;
			}
			rarity_rng -= Monster_Bases[i].stats[14];
		}
	}

	name = mb.name;
	for (int i = 0; i < 6; i++)
		stats[i] = mb.stats[2 * i] + lvl * stats[2 * i + 1];
	hp = stats[0];
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

	int stats[15];

	int i = 0;
	while (monster and i < 15)
	{
		monster >> stats[i];
		i++;
	}
	monster.close();

	Monster_Base mb(name, stats);
	Monster_Bases.push_back(mb);
}

