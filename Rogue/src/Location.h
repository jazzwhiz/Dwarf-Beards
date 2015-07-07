#ifndef Location_H
#define Location_H

#include "SDL/SDL.h"

#include <string>
#include <vector>

#include "Monster.h"

class World;

extern std::vector<SDL_Color> Location_Base_Colors;

class Location_Base
{
	public:
		Location_Base(std::string name, std::string letter, Uint8 _rgb[3], int width, int diff);
		std::string name, letter;
		Uint8 rgb[3];
		int width, diff;
};

class Location
{
	public:
		Location();
		std::string name;
		bool fog;
		SDL_Surface* to_surface();
		int diff, index, width, evil;
		std::vector<Monster> monsters;

		void update();

		// objects
};

void read_locations(World* w);
void read_location(World* w, std::string name);

#endif
