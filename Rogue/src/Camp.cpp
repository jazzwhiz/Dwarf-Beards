#include "SDL/SDL.h"

#include <string>
#include <vector>
#include <assert.h>

#include <iostream>

#include "Camp.h"
#include "Building.h"
#include "Draw.h"

const std::string Camp_Base_Names[3] = {"Empty", "Camp", "Town"};
SDL_Surface* Camp_Base_Surfaces[3];
int Camp_Base_widths[3];

Camp_Base::Camp_Base()
{
	Camp_Base(0);
}

SDL_Surface* Camp_Base::to_surface()
{
	return Camp_Base_Surfaces[index];
}

Camp_Base::Camp_Base(int index)
: index(index), name(Camp_Base_Names[index]), width(Camp_Base_widths[index])
{}

Camp::Camp()
: Camp_Base(1)
{
	buildings.push_back(Tavern());
	buildings.push_back(Inn());
}

Town::Town()
: Camp_Base(2)
{
	buildings.push_back(Tavern());
	buildings.push_back(Inn());
	buildings.push_back(Marketplace());
}

void _camp_init(Camp_Base cb)
{
	int i = 0;
	while (draw::font_sizes[i] != 28)
	{
		i++;
		assert (i < draw::num_fonts);
	}

	std::string letter;
	letter = cb.name[0];
	Camp_Base_Surfaces[cb.index] = TTF_RenderText_Blended(draw::fonts[i], letter.c_str(), draw::YELLOW);

	TTF_SizeText(draw::fonts[i], letter.c_str(), &Camp_Base_widths[cb.index], NULL);
}

void camp_init()
{
	_camp_init(Camp());
	_camp_init(Town());
}

