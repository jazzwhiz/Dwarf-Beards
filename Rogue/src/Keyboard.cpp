#include "SDL/SDL.h"

#include "Keyboard.h"
#include "Draw.h"
#include "Building.h"
#include "Timer.h"

namespace keyboard
{

// 0 - quit
// 1 - escape
// 2 - enter
int simple()
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
					case SDLK_ESCAPE:
						ret = 1;
						waiting = false;
						break;
					case SDLK_RETURN:
						ret = 2;
						waiting = false;
						break;
					default:
						break;
				}
			}
		}
		SDL_Flip(draw::screen);
		if (fps.get_ticks() < 1000 / draw::framerate)
		{
			SDL_Delay((1000 / draw::framerate) - fps.get_ticks());
		}
	} // waiting loop
	return ret;
}

// 0: quit
// 1: character screen
// 3-6: L, R, U, D
// 7: wait
// 8: battle
// 9: tavern
// 10: inn
// 11: help
int earth()
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
						break;
					case SDLK_t:
						waiting = false;
						ret = 9;
						break;
					case SDLK_i:
						waiting = false;
						ret = 10;
						break;
					case SDLK_SLASH:
						waiting = false;
						ret = 11;
						break;
					default:
						break;
				}
			}
		}
		SDL_Flip(draw::screen);
		if (fps.get_ticks() < 1000 / draw::framerate)
		{
			SDL_Delay((1000 / draw::framerate) - fps.get_ticks());
		}
	} // waiting loop
	return ret;
}

// 0: quit
// 1: atk
// 2: matk
// 4-13: 0-9, that is, take ret and subtract 4
int battle()
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
		SDL_Flip(draw::screen);
		if (fps.get_ticks() < 1000 / draw::framerate)
		{
			SDL_Delay((1000 / draw::framerate) - fps.get_ticks());
		}
	} // waiting loop
	return ret;
}

int building(Building_Base building)
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
				if (e.key.keysym.sym >= 48 and e.key.keysym.sym <= 57)
				{
					waiting = false;
					ret = e.key.keysym.sym - 48 + 2;
					break;
				}
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						ret = 1;
						waiting = false;
						break;
					default:
						break;
				}
			}
		}
		SDL_Flip(draw::screen);
		if (fps.get_ticks() < 1000 / draw::framerate)
		{
			SDL_Delay((1000 / draw::framerate) - fps.get_ticks());
		}
	} // waiting loop

	return ret;
}

} // namespace keyboard

