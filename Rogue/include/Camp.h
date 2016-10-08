#ifndef Camp_H
#define Camp_H

#include <vector>
#include <string>

#include "Building.h"

struct SDL_Surface;

class Camp_Base
{
	public:
		Camp_Base();
		Camp_Base(int index);

		int index;
		std::vector<Building_Base*> buildings;
		std::string name;

		SDL_Surface* to_surface();
		int width;
};

class Camp : public Camp_Base
{
	public:
		Camp();
};

class Town : public Camp_Base
{
	public:
		Town();
};

void _camp_init(Camp_Base cb);
void camp_init();

#endif
