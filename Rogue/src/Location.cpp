#include "SDL/SDL.h"

#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <math.h>

#include <iostream>

#include "Location.h"
#include "World.h"
#include "Draw.h"
#include "Monster.h"
#include "Battle.h"

#include "rng.h"

std::vector<Location_Base> Location_Bases;
std::vector<SDL_Color> Location_Base_Colors;
std::vector<SDL_Surface*> Location_Base_Surfaces;

const std::string evil_strings[11] =
	{"Serene", "Calm", "Simple", "Active", "Busy", "Happening", "Worrisome", "Scary", "Frightful", "Overrun", "Evil"};

Location_Base::Location_Base(std::string name, std::string letter, Uint8 _rgb[3], int width, int diff)
: name(name), letter(letter), width(width), diff(diff)
{
	for (int i = 0; i < 3; i++)
		rgb[i] = _rgb[i];
}

Location::Location()
: fog(true) // can't see by default
//: fog(false) // turn off fog
{
	index = rng.rand_int(Location_Bases.size() - 1);
	name = Location_Bases[index].name;
	diff = Location_Bases[index].diff;
	width = Location_Bases[index].width;
	evil = rng.rand_int(10);
}

const std::string Location::evil_str()
{
	return evil_strings[evil];
}

void Location::update()
{
	// spawn new monsters
	// powers closer to 0.1 favors lower levels, closer 0.9 favors a flatter distribution
	if (rng.rand() <= pow(evil / 10., 0.6) and rng.rand() < 0.07 and monsters.size() < 10)
		monsters.push_back(Monster(rng.rand_int(evil)));

	// monsters battle
	if (monsters.size() > 1)
	{
		if (rng.rand_int(20) < evil) // 0 evil = no battles, 10 evil = 50% battles
			Monster_Battle battle(&monsters);
	}

	// todo: move monsters?

	// heal monsters
	for (uint i = 0; i < monsters.size(); i++)
		monsters[i].heal((10 - evil) / 20.); // 0 evil = 0.5, 10 evil = 0

	// todo: grow trees, plants, update weather
}

SDL_Surface* Location::to_surface()
{
	return Location_Base_Surfaces[index];
}

void read_locations(World* w)
{
	std::string tmp, tmp2;
	std::ifstream names(w->data_dir + "Locations/Names");
	assert (names.is_open());

	while (names)
	{
		getline(names, tmp);
		if (tmp == "") continue;
		tmp2 = strdup(tmp.c_str());
		read_location(w, tmp2);
	}
	names.close();
}

void read_location(World* w, std::string name)
{
	std::ifstream location(w->data_dir + "Locations/" + name);
	assert (location.is_open());

	std::string letter;
	Uint8 rgb[3];
	int tmp, diff;

	location >> letter;
	for (int i = 0; i < 3; i++)
	{
		location >> tmp;
		rgb[i] = tmp;
	}
	location >> diff;
	location.close();

	int i = 0;
	while (draw::font_sizes[i] != 28)
	{
		i++;
		assert (i < draw::num_fonts);
	}

	SDL_Color color = {rgb[0], rgb[1], rgb[2]};

	Location_Base_Colors.push_back(color);
	Location_Base_Surfaces.push_back(TTF_RenderText_Blended(draw::fonts[i], letter.c_str(), color));

	int width;
	TTF_SizeText(draw::fonts[i], letter.c_str(), &width, NULL);

	Location_Base lb(name, letter, rgb, width, diff);
	Location_Bases.push_back(lb);
}

