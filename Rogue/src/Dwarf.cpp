#include <string>

#include <iostream>

#include "Dwarf.h"

std::string stat_names[5] = {"HP", "ATK", "MATK", "DEF", "MDEF"};

Dwarf::Dwarf(std::string name)
: name(name), beard(1), exp(0), thirst(0), sleepiness(0)
{

	for (int i = 0; i < 5; i++)
		stats[i] = 10;
	hp = stats[0];
}

void Dwarf::gain_exp(int _exp)
{
	exp += _exp;
	while (exp >= 50 * beard * beard)
	{
		exp -= 50 * beard * beard;
		beard++;
		stats[0] += 5;
		for (int i = 1; i < 5; i++)
			stats[i] += 2;
	}
}

