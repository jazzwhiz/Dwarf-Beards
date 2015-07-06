#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "World.h"
#include "Dwarf.h"
#include "Timer.h"
#include "Earth.h"
#include "rng.h"

const int framerate = 60;
Timer fps;
SDL_Event e;
SDL_Surface *screen;
SDL_Surface *message;
SDL_Surface *square_large;
SDL_Surface *square_small;

// Some colors
// Grayscale
const SDL_Color World::WHITE = {255, 255, 255};
const SDL_Color World::LIGHT_GRAY = {150, 150, 150};
const SDL_Color World::GRAY = {90, 90, 90};
const SDL_Color World::DARK_GRAY = {60, 60, 60};
const SDL_Color World::BLACK = {0, 0, 0};
// Others
const SDL_Color World::BROWN = {139, 69, 19};
const SDL_Color World::GOLD = {255, 215, 0};
const SDL_Color World::RED = {255, 0, 0};
const SDL_Color World::DARK_BLUE = {0, 0, 100};
const SDL_Color World::YELLOW = {255, 255, 0};

// initialize rng
int seed = 1888;
RNG rng = RNG(seed);
//RNG rng = RNG();

World::World()
{
	// screen parameter stuff
	screen_size[0] = 800;
	screen_size[1] = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(screen_size[0], screen_size[1], 32, SDL_SWSURFACE);

SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
std::cout << SDL_DEFAULT_REPEAT_DELAY << " " << SDL_DEFAULT_REPEAT_INTERVAL << std::endl;

	// bare tiles
	square_large = SDL_CreateRGBSurface(0, 10, 10, 32, 0, 0, 0, 0);
	square_small = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);

	// set up bools
	running = true;
	paused = false;

	version = "0.04";
	copyright = "(c) 2014-2015 Peter Denton";

	init_names();

	earth = Earth();
	earth.init();

	focus[0] = earth.earth_size[0] / 2;
	focus[1] = earth.earth_size[1] / 2;
	focus[2] = 0;

	run();
	clean_up();
}

void World::text(const std::string msg, int size, SDL_Color color, int x, int y, bool centerx)
{
	TTF_Font *font = NULL;
	font = TTF_OpenFont("Font/sfd/FreeSans.ttf", size);

//	message = TTF_RenderText_Solid(font, msg.c_str(), color);
//	message = TTF_RenderText_Shaded(font, msg.c_str(), color, BLACK);
	message = TTF_RenderText_Blended(font, msg.c_str(), color);

	if (centerx)
	{
		int w;
		TTF_SizeText(font, msg.c_str(), &w, NULL);
		x = x - w / 2;
	}

	apply_surface(x, y, message, screen);
	TTF_CloseFont(font);
}

void World::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	
	// Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;
	// Blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}

void World::draw_main()
{
	draw_earth();
	draw_sidebar();
	draw_buildings();
	draw_dwarves();
	SDL_Flip(screen);
}

void World::draw_tile(int x, int y, Location location)
{
	if (location.floor)
	{
		SDL_FillRect(square_small, NULL, SDL_MapRGB(square_small->format, location.color.r, location.color.g, location.color.b));
		apply_surface(12 * x + 1, 12 * y + 1, square_small, screen);
	}
	else if (not location.empty)
	{
		SDL_FillRect(square_large, NULL, SDL_MapRGB(square_large->format, location.color.r, location.color.g, location.color.b));
		apply_surface(12 * x, 12 * y, square_large, screen);
	}
}

void World::draw_earth()
{
	for (int x = 0; x < earth.earth_size[0]; x++)
	{
		for (int y = 0; y < earth.earth_size[1]; y++)
		{
			draw_tile(x, y, earth.earth[x][y][focus[2]]);
		} // y
	} // x
}

void World::draw_buildings()
{

}

void World::draw_dwarves()
{
	for (uint i = 0; i < dwarves.size(); i++)
	{
		if (dwarves[i].loc[2] == focus[2])
		{
			text("@", 9, RED, 12 * dwarves[i].loc[0] + 5, 12 * dwarves[i].loc[1] - 3, true);
		}
	}
}

void World::draw_sidebar()
{
	std::string tmp;
	tmp = "Location: " + std::to_string(focus[0]) + " " + std::to_string(focus[1]) + " " + std::to_string(focus[2]);
	text(tmp, 15, RED, 680, 580, false);
}

void World::run()
{
	fps.start();

	title();
	clear_screen();

	run_main();
}

void World::run_main()
{
	int local_ticks;

	while (running)
	{
		local_ticks = fps.get_ticks();

		clear_screen();
		draw_main();

//		SDL_WaitEvent(&e);
		SDL_PollEvent(&e); 
		if(e.type == SDL_QUIT)
		{
			running = false;
			std::cout << "Quitting..." << std::endl;
		}

		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_ESCAPE: // exits the game (for now)
					running = false;
					break;
				case SDLK_PERIOD: // ">" takes us down a z level into the earth
					if (e.key.keysym.mod and KMOD_SHIFT)
						focus[2]++;
					break;
				case SDLK_COMMA: // "<" takes us up a z level out of the earth
					if (e.key.keysym.mod and KMOD_SHIFT)
						focus[2]--;
					break;
				case SDLK_n:
					dwarves.push_back(Dwarf(random_name(), focus));
					std::cout << "Added new dwarf #" << dwarves.size() << " named " << dwarves.back().name << std::endl;
					break;
				default:
					break;
			}
		}

		update();

		if(fps.get_ticks() - local_ticks < 1000 / framerate)
		{
			SDL_Delay((1000 / framerate) - fps.get_ticks() + local_ticks);
		}

	}
}

void World::title()
{
	bool titling = true;

	text("jazzwhiz games", 30, WHITE, screen_size[0] / 2, 40, true);
	text("presents", 20, WHITE, screen_size[0] / 2, 100, true);
	text("DWARF BEARDS", 50, WHITE, screen_size[0] / 2, 300, true);
	text("Version " + version + " " + copyright, 15, DARK_GRAY, 529, 580, false);

	while (titling and running)
	{
		SDL_PollEvent(&e); 
		if(e.type == SDL_QUIT)
		{
			running = false;
			std::cout << "Quitting..." << std::endl;
		}
		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_RETURN:
					titling = false;
					break;
				default:
					break;
			}
		}
		SDL_Flip(screen);
		if(fps.get_ticks() < 1000 / framerate)
		{
			SDL_Delay((1000 / framerate) - fps.get_ticks());
		}
	}
}

void World::init_names()
{
	std::ifstream firstfile("../../data/first.txt");
	std::ifstream lastfile("../../data/last.txt");

	std::string tmp;
	std::string tmp2;
	if (firstfile.is_open())
	{
		while (firstfile)
		{
			getline(firstfile, tmp);
			if (tmp == "") continue;
			tmp2 = strdup(tmp.c_str());
			firsts.push_back(tmp2);
		}
	}
	if (lastfile.is_open())
	{
		while (lastfile)
		{
			getline(lastfile,tmp);
			if (tmp == "") continue;
			tmp2=strdup(tmp.c_str());
			lasts.push_back(tmp2);
		}
	}
	firstfile.close();
	lastfile.close();
}

std::string World::random_name()
{
	return firsts[rng.rand_int(firsts.size() - 1)] + " " + lasts[rng.rand_int(lasts.size() - 1)];
}

void World::update()
{
	// focus: cyclic world
	for (int i = 0; i < 3; i++)
	{
		if (focus[i] > earth.earth_size[i] - 1)
			focus[i] = 0;
		if (focus[i] < 0)
			focus[i] = earth.earth_size[i] - 1;
	}

	// update dwarves
	for (uint i = 0; i < dwarves.size(); i++)
		dwarves[i].update();
}

void World::clear_screen()
{
	SDL_FillRect(screen, NULL, 0);
}

void World::clean_up()
{
	std::cout << "Cleaning up..." << std::endl;
	SDL_FreeSurface(message);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(square_large);
	SDL_FreeSurface(square_small);

	TTF_Quit();
	SDL_Quit();	
}

