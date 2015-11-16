#include "Modification.h"
#include "Dwarf.h"

Drink::Drink()
{
	cost = 15;
	description = "Order up some drinks";
}

void Drink::modify(Dwarf* player)
{
	player->gold -= cost;
	player->thirst = 0;
}

Sleep::Sleep()
{
	cost = 40;
	description = "Spend the night";
}

void Sleep::modify(Dwarf* player)
{
	player->gold -= cost;
	player->sleepiness = 0;
}

