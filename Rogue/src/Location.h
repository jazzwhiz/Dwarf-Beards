#ifndef Location_H
#define Location_H

#include "SDL/SDL.h"

#include <string>

class World;

class Location_Base
{
	public:
		Location_Base(std::string name, std::string letter, Uint8 _rgb[3], int diff);
		std::string name, letter;
		Uint8 rgb[3];
		int diff;
};

class Location
{
	public:
		Location();
		std::string name;
		bool fog;
		SDL_Surface* to_surface();
		int diff, index;

		// objects
};

void read_locations(World* w);
void read_location(World* w, std::string name);

#endif
