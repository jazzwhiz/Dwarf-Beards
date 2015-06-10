#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "Earth.h"

class World
{
	public:
		// constructor
		World();

		// allegro stuff
		int screen_size[2];
		int frame_rate;

		// set up stuff
		std::vector<std::string> firsts;
		std::vector<std::string> lasts;
		int earth_size[3];
		Earth earth;
		
		void p(const std::string msg,int priority);

		std::string version;
		std::string copyright;

		// Some colors
		// Grayscale
		static const SDL_Color WHITE;
		static const SDL_Color LIGHT_GRAY;
		static const SDL_Color GRAY;
		static const SDL_Color DARK_GRAY;
		static const SDL_Color BLACK;
	
		// Others
		static const SDL_Color BROWN;
		static const SDL_Color GOLD;
		static const SDL_Color RED;
		static const SDL_Color DARK_BLUE;
		static const SDL_Color YELLOW;

	protected:
		void run();
		void draw_world();
		void draw_fs();

		bool running;
		bool paused;

		void text(const std::string msg,int size,SDL_Color color,int x,int y,bool centerx);
		std::string random_name();

	private:
		void init_names();
		int seed;
		void apply_surface(int x,int y,SDL_Surface* source,SDL_Surface* destination);

		void title();
		void clean_up();

};
#endif
