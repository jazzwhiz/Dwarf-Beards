#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include <string>
#include <algorithm>
#include <assert.h>

#include <iostream>

#include "Draw.h"
#include "Timer.h"

namespace draw
{

const int framerate = 60;
const int screen_size[2] = {800, 600};
Timer fps;
SDL_Event e;
SDL_Surface *screen;
SDL_Surface *message;

// Some colors
// Grayscale
const SDL_Color WHITE = {255, 255, 255};
const SDL_Color LIGHT_GRAY = {150, 150, 150};
const SDL_Color GRAY = {90, 90, 90};
const SDL_Color DARK_GRAY = {60, 60, 60};
const SDL_Color BLACK = {0, 0, 0};
// Others
const SDL_Color BROWN = {139, 69, 19};
const SDL_Color GOLD = {255, 215, 0};
const SDL_Color RED = {255, 0, 0};
const SDL_Color DARK_BLUE = {0, 0, 100};
const SDL_Color YELLOW = {255, 255, 0};

const int num_fonts = 3;
const int font_sizes[num_fonts] = {12, 16, 72};
TTF_Font* fonts[num_fonts];

void title(World w)
{
	bool titling = true;

	text("Dwarf Beards:", 72, LIGHT_GRAY, screen_size[0] / 2, 50, 1);
	text("Rogue", 72, RED, screen_size[0] / 2, 122, 1);
	text("Press enter to start", 16, LIGHT_GRAY, screen_size[0] / 2, 350, 1);
	text("Version " + w.version + " " + w.copyright, 12, DARK_GRAY, screen_size[0], screen_size[1] - 16, 2);

	while (titling)
	{

		SDL_PollEvent(&e); 
		if (e.type == SDL_QUIT or e.key.keysym.sym == SDLK_ESCAPE)
		{
			titling = false;
			w.quit();
		}
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_RETURN:
					titling = false;
					break;
				default:
					break;
			}
		}
		SDL_Flip(screen);
		if (fps.get_ticks() < 1000 / framerate)
		{
			SDL_Delay((1000 / framerate) - fps.get_ticks());
		}


	}

}

// alignx: 0 - left, 1 - center, 2 - right
void text(const std::string msg, int size, SDL_Color color, int x, int y, int alignx)
{
	int index = 0;
	while (font_sizes[index] != size)
	{
		index++;
		assert (index < num_fonts);
	}

//	message = TTF_RenderText_Solid(font, msg.c_str(), color);
//	message = TTF_RenderText_Shaded(font, msg.c_str(), color, BLACK);
	message = TTF_RenderText_Blended(fonts[index], msg.c_str(), color);

	if (alignx > 0)
	{
		int w;
		TTF_SizeText(fonts[index], msg.c_str(), &w, NULL);
		if (alignx == 1)
			x = x - w / 2;
		if (alignx == 2)
			x = x - w;
	}

	apply_surface(x, y, message, screen);
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	
	// Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;
	// Blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}

void init(World w)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(screen_size[0], screen_size[1], 32, SDL_SWSURFACE);

	std::string font_file = w.data_dir + "Font/sfd/FreeSans.ttf";
	for (int i = 0; i < num_fonts; i++)
		fonts[i] = TTF_OpenFont(font_file.c_str(), font_sizes[i]);

}

void clean_up()
{
	for (int i = 0; i < num_fonts; i++)
		TTF_CloseFont(fonts[i]);
}

} // namespace draw
