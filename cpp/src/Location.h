#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include "SDL/SDL.h"

struct lid_def
{
	int lid;
	std::string name;
	int diff;
	SDL_Color color;
};

class Location
{
	public:
		bool floor;
		bool empty;
		std::string name;
		int diff;
		SDL_Color color;
		int lid;
	protected:
		void init();
};

class Soil:Location
{
	public:
		Soil();
		void init();
		int asdf;
};

#endif
