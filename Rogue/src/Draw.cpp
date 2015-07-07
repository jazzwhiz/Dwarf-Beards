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
#include "Dwarf.h"
#include "World.h"
#include "Location.h"


namespace draw
{

const int framerate = 32;
const int screen_size[2] = {800, 600};
SDL_Surface* screen;
SDL_Surface* message;
SDL_Surface* player_surface; // the at symbol

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
const SDL_Color BLUE = {0, 0, 255};
const SDL_Color DARK_BLUE = {0, 0, 100};
const SDL_Color YELLOW = {255, 255, 0};

// hp, atk, matk, def, mdef
SDL_Color stat_colors[5] = {DARK_BLUE, RED, YELLOW, BROWN, WHITE};

const int num_fonts = 6;
const int font_sizes[num_fonts] = {12, 14, 16, 24, 28, 72};
TTF_Font* fonts[num_fonts];

void title(World* w)
{
	clear_screen();

	text("Dwarf Beards:", 72, LIGHT_GRAY, screen_size[0] / 2, 150, 1);
	text("Rogue", 72, RED, screen_size[0] / 2, 252, 1);
	text("Press enter to start", 16, LIGHT_GRAY, screen_size[0] / 2, 570, 1);
	text("Version " + w->version + " " + w->copyright, 12, DARK_GRAY, screen_size[0], screen_size[1] - 16, 2);

	if (wait_static())
		dwarf_profile(w);
}

void dwarf_profile(World* w)
{
	clear_screen();

	std::string tmp;
	int y = 40;

	tmp = w->player.name;
	text(tmp, 24, LIGHT_GRAY, screen_size[0] / 2, y, 1);
	y += 30;

	tmp = "Beard length: " + std::to_string(w->player.beard);
	tmp += "   Exp: " + std::to_string((int)(100 * (double)(w->player.exp) / (50 * w->player.beard * w->player.beard))) + "%";
	text(tmp, 16, LIGHT_GRAY, screen_size[0] / 2, y, 1);
	y += 30;

	tmp = stat_names[0] + ": " + std::to_string((int)w->player.hp) + "/" + std::to_string(w->player.stats[0]);
	tmp += "  Thirst: " + std::to_string((int)(100 * w->player.thirst)) + "%";
	tmp += "  Sleepiness: " + std::to_string((int)(100 * w->player.sleepiness)) + "%";
	text(tmp, 16, stat_colors[0], screen_size[0] / 2, y, 1);
	y += 30;

	for (int i = 1; i < 5; i++)
	{
		tmp = stat_names[i] + ": " + std::to_string(w->player.stats[i]);
		text(tmp, 16, stat_colors[i], screen_size[0] / 2, y, 1);
		y += 30;
	}

	// todo: equipment

	if (wait_static())
		earth(w);
}

void earth(World* w)
{
	clear_screen();

	// map x < 600
	for (int x = 0; x < w->earth->earth_size[0]; x++)
	{
		for (int y = 0; y < w->earth->earth_size[1]; y++)
		{
			if (not w->earth->locations[x][y].fog)
			{
				if (x == w->location[0] and y == w->location[1])
					apply_surface(28 * x + 6, 28 * y, player_surface, screen);
				else
					apply_surface(28 * x + 20 - w->earth->locations[x][y].width / 2, 28 * y, w->earth->locations[x][y].to_surface(), screen);
			}
		} // y
	} // x

	// sidebar x > 600
	int x = 605;
	int y = 20;
	std::string tmp;

	// turn number
	text("Turn: " + std::to_string(w->turn), 16, LIGHT_GRAY, x, y, 0);
	y += 30;

	// player info
	text(w->player.name, 16, LIGHT_GRAY, x, y, 0);
	y += 22;

	text(stat_names[0] + ": " + std::to_string((int)w->player.hp) + "/" + std::to_string(w->player.stats[0]), 14, LIGHT_GRAY, x + 10, y, 0);
	y += 22;

	text("Thirst: " + std::to_string((int)(100 * w->player.thirst)) + "%", 14, LIGHT_GRAY, x + 10, y, 0);
	y += 22;

	text("Sleepiness: " + std::to_string((int)(100 * w->player.sleepiness)) + "%", 14, LIGHT_GRAY, x + 10, y, 0);
	y += 30;

	// location info
	text("Location: (" + std::to_string(w->location[0] - 10) + "," + std::to_string(w->location[1] - 10) + ")", 16, LIGHT_GRAY, x, y, 0);
	y += 22;

	text(w->earth->locations[w->location[0]][w->location[1]].name, 14, Location_Base_Colors[w->earth->locations[w->location[0]][w->location[1]].index], x + 10, y, 0);

	if (wait_static())
		y++; // nothing
		// next
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
		int width;
		TTF_SizeText(fonts[index], msg.c_str(), &width, NULL);
		if (alignx == 1)
			x -= width / 2;
		if (alignx == 2)
			x -= width;
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

void clear_screen()
{
	SDL_FillRect(screen, NULL, 0);
}

bool wait_static()
{
	bool waiting = true;
	bool playing = true;
	SDL_Event e;
	Timer fps;
	while (waiting)
	{
		fps.start();
		while(SDL_PollEvent(&e));
		{
			if (e.type == SDL_QUIT or e.key.keysym.sym == SDLK_ESCAPE)
			{
				waiting = false;
				playing = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_RETURN:
						waiting = false;
					default:
						break;
				}
			}
		}
		SDL_Flip(screen);
		if (fps.get_ticks() < 1000 / framerate)
		{
			SDL_Delay((1000 / framerate) - fps.get_ticks());
		}
	} // waiting loop
	return playing;
}

bool clear_events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e));
	{
		std::cout << "  ..." << &e.key.keysym.sym << std::endl;
		if (e.type == SDL_QUIT or e.key.keysym.sym == SDLK_ESCAPE)
		{
			return false;
		}
	}

	return true;

}

void init(World* w)
{
	// start all sdl stuff
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(screen_size[0], screen_size[1], 32, SDL_SWSURFACE);

	// set up window
	SDL_WM_SetCaption("Dwarf Beards: Rogue", "");

	// initialize fonts
	std::string font_file = w->data_dir + "Font/sfd/FreeSans.ttf";
	for (int i = 0; i < num_fonts; i++)
		fonts[i] = TTF_OpenFont(font_file.c_str(), font_sizes[i]);

	// initialize player surface
	int i = 0;
	while (draw::font_sizes[i] != 28)
	{
		i++;
		assert (i < draw::num_fonts);
	}
	
	player_surface = TTF_RenderText_Blended(fonts[i], "@", RED);

}

void clean_up()
{
	std::cout << "Cleaning up sdl..." << std::endl;
	for (int i = 0; i < num_fonts; i++)
		TTF_CloseFont(fonts[i]);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(message);
	SDL_Quit();
}

} // namespace draw
