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
struct lid_def tmp_lid={0,"Soil",1,World::BROWN};
struct lid_def lids [5]={tmp_lid,tmp_lid,tmp_lid,tmp_lid,tmp_lid};

//lids.lid=0;

void Location::init()
{
	floor=false;
	empty=false;
}
Soil::Soil()
{
	Location::init();
	lid = 0;
	name = "Soil";
	diff = 1;
//	color = World::BROWN;
}
