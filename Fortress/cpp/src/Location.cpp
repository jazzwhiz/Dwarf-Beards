#include "Location.h"
#include "World.h"
/*
lids[0]=
{
	(0,	"Soil",		1,	World::BROWN),
	(1,	"Bedrock",	6,	World::DARK_GRAY),
	(2,	"Silver",	4,	World::LIGHT_GRAY),
	(3,	"Gold",		3,	World::GOLD),
	(4,	"Iron",		8,	World::GRAY)
};*/

void Location::init()
{
	floor = false;
	empty = false;
}
Soil::Soil()
{
	Location::init();
	lid = 0;
	name = "Soil";
	diff = 1;
	color = World::BROWN;
}
Bedrock::Bedrock()
{
	Location::init();
	lid = 1;
	name = "Bedrock";
	diff = 6;
	color = World::DARK_GRAY;
}
Silver::Silver()
{
	Location::init();
	lid = 2;
	name = "Silver";
	diff = 4;
	color = World::LIGHT_GRAY;
}
Gold::Gold()
{
	Location::init();
	lid = 3;
	name = "Gold";
	diff = 3;
	color = World::GOLD;
}
Iron::Iron()
{
	Location::init();
	lid = 4;
	name = "Iron";
	diff = 8;
	color = World::GRAY;
}

Location Location_from_lid(int lid)
{
	switch (lid)
	{
		case 0:
			return Soil();
		case 1:
			return Bedrock();
		case 2:
			return Silver();
		case 3:
			return Gold();
		case 4:
			return Iron();
		default:
			return Bedrock(); // defaults to bedrock

	}		
}


