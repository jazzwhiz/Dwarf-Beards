#include "SDL/SDL.h"

#include <string>
#include <vector>
#include <fstream>
#include <assert.h>

#include <iostream>

#include "Location.h"
#include "World.h"
#include "Draw.h"

#include "rng.h"

std::vector<Location_Base> Location_Bases;
std::vector<SDL_Surface*> Location_Base_Surfaces;

Location_Base::Location_Base(std::string name, std::string letter, Uint8 _rgb[3], int diff)
: name(name), letter(letter), diff(diff)
{
	for (int i = 0; i < 3; i++)
		rgb[i] = _rgb[i];
}

Location::Location()
: fog(true) // can't see by default
{
	index = rng.rand_int(Location_Bases.size() - 1);
	name = Location_Bases[index].name;
	diff = Location_Bases[index].diff;
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

	Location_Base lb(name, letter, rgb, diff);
	Location_Bases.push_back(lb);

	int i = 0;
	while (draw::font_sizes[i] != 28)
	{
		i++;
		assert (i < draw::num_fonts);
	}

	SDL_Color color = {rgb[0], rgb[1], rgb[2]};


	Location_Base_Surfaces.push_back(TTF_RenderText_Blended(draw::fonts[i], letter.c_str(), color));
}

