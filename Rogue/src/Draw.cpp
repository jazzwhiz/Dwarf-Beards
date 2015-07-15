#include "SDL/SDL.h"
//#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
//#include "SDL/SDL_mixer.h"

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
SDL_Surface* player_surface; // D

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
SDL_Color stat_colors[5] = {BLUE, RED, YELLOW, BROWN, WHITE};

const int num_fonts = 6;
const int font_sizes[num_fonts] = {12, 14, 16, 24, 28, 72};
TTF_Font* fonts[num_fonts];

bool title(World* w)
{
	clear_screen();

	text("Dwarf Beards:", 72, LIGHT_GRAY, screen_size[0] / 2, 150, 1);
	text("Rogue", 72, RED, screen_size[0] / 2, 252, 1);
	text("Press enter to start", 16, LIGHT_GRAY, screen_size[0] / 2, 570, 1);
	text("Version " + w->version + " " + w->copyright, 12, DARK_GRAY, screen_size[0], screen_size[1] - 16, 2);

	return wait_static();
}

int dwarf_profile(World* w)
{
	clear_screen();

	std::string tmp;
	int y = 40;

	tmp = w->player.name;
	text(tmp, 24, LIGHT_GRAY, screen_size[0] / 2, y, 1);
	y += 30;

	tmp = "Beard length: " + std::to_string(w->player.lvl);
	tmp += "   Exp: " + std::to_string((int)(100 * (double)(w->player.exp) / (50 * (w->player.lvl + 1) * (w->player.lvl + 1)))) + "%";
	text(tmp, 16, LIGHT_GRAY, screen_size[0] / 2, y, 1);
	y += 30;

	tmp = stat_names[0] + ": " + std::to_string((int)w->player.hp) + "/" + std::to_string(w->player.stats[0]);
	tmp += "  Thirst: " + std::to_string((int)(100 * w->player.thirst)) + "%";
	tmp += "  Sleepiness: " + std::to_string((int)(100 * w->player.sleepiness)) + "%";
	text(tmp, 16, stat_colors[0], screen_size[0] / 2, y, 1);
	y += 30;

	text("Gold: " + std::to_string(w->player.gold), 16, YELLOW, screen_size[0] / 2, y, 1);
	y += 30;

	for (int i = 1; i < 5; i++)
	{
		tmp = stat_names[i] + ": " + std::to_string(w->player.stats[i]);
		text(tmp, 16, stat_colors[i], screen_size[0] / 2, y, 1);
		y += 30;
	}

	// todo: equipment

	return wait_static() ? 2 : 0;
}

int earth(World* w)
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
					apply_surface(28 * x + 11, 28 * y, player_surface, screen);
				else if (w->earth->locations[x][y].camp.index != 0)
				{
					apply_surface(28 * x + 20 - w->earth->locations[x][y].camp.width / 2, 28 * y,
									w->earth->locations[x][y].camp.to_surface(), screen);
//					std::cout << w->earth->locations[x][y].camp.width << std::endl;
				}
				else
					apply_surface(28 * x + 20 - w->earth->locations[x][y].width / 2, 28 * y, w->earth->locations[x][y].to_surface(), screen);
			}
		} // y
	} // x

	// sidebar x > 600
	int x = 605;
	int y = 20;

	// turn number
	text("Turn: " + std::to_string(w->turn), 16, LIGHT_GRAY, x, y, 0);
	y += 25;

	// player info
	text(w->player.name + " (" + std::to_string(w->player.lvl) + ")", 16, LIGHT_GRAY, x, y, 0);
	y += 22;

	text(stat_names[0] + ": " + std::to_string((int)w->player.hp) + "/" + std::to_string(w->player.stats[0]), 14, LIGHT_GRAY, x + 10, y, 0);
	y += 18;

	text("Thirst: " + std::to_string((int)(100 * w->player.thirst)) + "%", 14, LIGHT_GRAY, x + 10, y, 0);
	y += 18;

	text("Sleepiness: " + std::to_string((int)(100 * w->player.sleepiness)) + "%", 14, LIGHT_GRAY, x + 10, y, 0);
	y += 18;

	text("Gold: " + std::to_string(w->player.gold), 14, YELLOW, x + 10, y, 0);
	y += 25;

	// location info
	text("Location: (" + std::to_string(w->location[0] - 10) + "," + std::to_string(w->location[1] - 10) + ")", 16, LIGHT_GRAY, x, y, 0);
	y += 22;

	text(w->earth->locations[w->location[0]][w->location[1]].name, 14, Location_Base_Colors[w->earth->locations[w->location[0]][w->location[1]].index], x + 10, y, 0);
	y += 18;

	text(w->earth->locations[w->location[0]][w->location[1]].evil_str(), 14, LIGHT_GRAY, x + 10, y, 0);
	y += 25;

	// buildings
	int n_buildings = w->earth->locations[w->location[0]][w->location[1]].camp.buildings.size();
	if (n_buildings > 0)
	{
		text(w->earth->locations[w->location[0]][w->location[1]].camp.name, 16, LIGHT_GRAY, x, y, 0);
		y += 22;
		for (int i = 0; i < n_buildings; i++)
		{
			text(w->earth->locations[w->location[0]][w->location[1]].camp.buildings[i].name, 14, LIGHT_GRAY, x + 10, y, 0);
			y += 18;
		}
	}
	y += 7;

	// monster info
	int n_monsters = w->earth->locations[w->location[0]][w->location[1]].monsters.size();
	if (n_monsters >= 1)
	{
		text("Monsters:", 16, LIGHT_GRAY, x, y, 0);
		y += 22;
	}
	for (int i = 0; i < n_monsters; i++)
	{
		std::string tmp;
		tmp = "  " + w->earth->locations[w->location[0]][w->location[1]].monsters[i].name + "(";
		tmp += std::to_string(w->earth->locations[w->location[0]][w->location[1]].monsters[i].lvl) + ") ";
		tmp += std::to_string((int)w->earth->locations[w->location[0]][w->location[1]].monsters[i].hp) + "/";
		tmp += std::to_string(w->earth->locations[w->location[0]][w->location[1]].monsters[i].stats[0]);
		text(tmp, 14, LIGHT_GRAY, x + 10, y, 0);
		y += 18;
	}

	return wait_earth();
}

void battle(World* w, int attack_style, int attack_target, std::vector<std::string> readout)
{
	clear_screen();

	int x;
	int y = 10;
	int n_monsters = w->earth->locations[w->location[0]][w->location[1]].monsters.size();
	std::string tmp;

	text("BATTLE", 24, RED, screen_size[0] / 2, y, 1);
	y += 30;

	// left panel
	x = 20;
	tmp = w->player.name + "(" + std::to_string(w->player.lvl) + ") HP: ";
	tmp += std::to_string((int)w->player.hp) + "/" + std::to_string(w->player.stats[0]);
	text(tmp, 16, RED, x, y, 0);
	y += 25;

	x += 10;
	for (int i = 0; i < n_monsters; i++)
	{
		tmp = std::to_string(i) + " - ";
		tmp += w->earth->locations[w->location[0]][w->location[1]].monsters[i].name + "(";
		tmp += std::to_string(w->earth->locations[w->location[0]][w->location[1]].monsters[i].lvl) + ") HP: ";
		tmp += std::to_string((int)w->earth->locations[w->location[0]][w->location[1]].monsters[i].hp) + "/";
		tmp += std::to_string(w->earth->locations[w->location[0]][w->location[1]].monsters[i].stats[0]);
		text(tmp, 16, (attack_target == i) ? WHITE : LIGHT_GRAY, x, y, 0);
		y += 18;
	}

	int width;
	width = text("ATK ", 16, (attack_style == 0) ? WHITE : LIGHT_GRAY, 4, screen_size[1] - 24, 0);
	text("MATK ", 16, (attack_style == 1) ? WHITE : LIGHT_GRAY, 4 + width, screen_size[1] - 24, 0);

	// right panel
	// battle action
	x = screen_size[0] / 2 + 5;
	y = 50;
	for (unsigned i = 0; i < readout.size(); i++)
	{
		text(readout[i], 16, LIGHT_GRAY, x, y, 0);
		y += 18;
	}

}

// alignx: 0 - left, 1 - center, 2 - right
int text(const std::string msg, int size, SDL_Color color, int x, int y, int alignx)
{
	int index = 0;
	int width;
	while (font_sizes[index] != size)
	{
		index++;
		assert (index < num_fonts);
	}

	message = TTF_RenderText_Blended(fonts[index], msg.c_str(), color);

	TTF_SizeText(fonts[index], msg.c_str(), &width, NULL);
	if (alignx > 0)
	{
		if (alignx == 1)
			x -= width / 2;
		if (alignx == 2)
			x -= width;
	}

	apply_surface(x, y, message, screen);
	return width;
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
			if (e.type == SDL_QUIT)
			{
				waiting = false;
				playing = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_RETURN:
					case SDLK_ESCAPE:
						waiting = false;
						break;
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

// 0: quit
// 1: character screen
// 2-5: L, R, U, D
int wait_earth()
{
	bool waiting = true;
	int ret = 0;
	SDL_Event e;
	Timer fps;
	while (waiting)
	{
		fps.start();
		while(SDL_PollEvent(&e));
		{
			if (e.type == SDL_QUIT)
			{
				waiting = false;
				ret = 0;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_c:
						waiting = false;
						ret = 1;
						break;
					case SDLK_LEFT:
						waiting = false;
						ret = 3;
						break;
					case SDLK_RIGHT:
						waiting = false;
						ret = 4;
						break;
					case SDLK_UP:
						waiting = false;
						ret = 5;
						break;
					case SDLK_DOWN:
						waiting = false;
						ret = 6;
						break;
					case SDLK_w:
						waiting = false;
						ret = 7;
						break;
					case SDLK_b:
						waiting = false;
						ret = 8;
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
	return ret;
}

// 0: quit
// 1: atk
// 2: matk
// 4-13: 0-9, that is, take ret and subtract 4
int wait_battle()
{
	bool waiting = true;
	int ret = 0;
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
				ret = 0;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym >= 48 and e.key.keysym.sym <= 57)
				{
					waiting = false;
					ret = e.key.keysym.sym - 48 + 4;
					break;
				}
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						waiting = false;
						ret = 1;
						break;
					case SDLK_m:
						waiting = false;
						ret = 2;
						break;
					case SDLK_RETURN:
						waiting = false;
						ret = 3;
						break;
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
	return ret;
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
	
	player_surface = TTF_RenderText_Blended(fonts[i], "D", RED);

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
