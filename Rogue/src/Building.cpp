#include <string>
#include <vector>

#include "Building.h"
#include "Dwarf.h"
#include "Modification.h"

Tavern::Tavern()
{
	name = "Tavern";
	capacity = 10;

	Modification_Base* drink = new Drink();
	modifications.push_back(drink);
}

Inn::Inn()
{
	name = "Inn";
	capacity = 5;

	Modification_Base* sleep = new Sleep();
	modifications.push_back(sleep);
}

Marketplace::Marketplace()
{
	name = "Marketplace";
	capacity = 8;
}

