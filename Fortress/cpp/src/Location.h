#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include "SDL/SDL.h"

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

class Soil : public Location
{
	public:
		Soil();
};
class Bedrock : public Location
{
	public:
		Bedrock();
};
class Silver : public Location
{
	public:
		Silver();
};
class Gold : public Location
{
	public:
		Gold();
};
class Iron : public Location
{
	public:
		Iron();
};

Location Location_from_lid(int lid);

#endif
