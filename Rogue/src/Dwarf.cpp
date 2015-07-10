#include <string>

#include <iostream>

#include "Dwarf.h"

std::string stat_names[5] = {"HP", "ATK", "MATK", "DEF", "MDEF"};

Dwarf::Dwarf(std::string name)
: name(name), beard(0), exp(0), gold(100), thirst(0), sleepiness(0)
{

	stats[0] = 50;
	for (int i = 1; i < 5; i++)
		stats[i] = 10;
	hp = stats[0];
}

void Dwarf::gain_exp(int _exp)
{
	exp += _exp;
	while (exp >= 50 * (beard + 1) * (beard + 1))
	{
		exp -= 50 * (beard + 1) * (beard + 1);
		beard++;
		stats[0] += 5;
		for (int i = 1; i < 5; i++)
			stats[i] += 2;
	}
}

