#include <string>

#include "Building.h"
#include "Dwarf.h"

Tavern::Tavern()
: cost(15)
{
	name = "Tavern";
	capacity = 10;
}

void Tavern::drink(Dwarf* player)
{
	player->gold -= cost;
	player->thirst = 0;
}

Inn::Inn()
: cost(40)
{
	name = "Inn";
	capacity = 7;
}

void Inn::sleep(Dwarf* player)
{
	player->gold -= cost;
	player->sleepiness = 0;
}

Marketplace::Marketplace()
{
	name = "Marketplace";
	capacity = 8;
}

