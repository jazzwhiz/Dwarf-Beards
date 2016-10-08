#ifndef World_H
#define World_H

#include <string>
#include <vector>

#include "Dwarf.h"
#include "Earth.h"

class World
{
	public:
		World();
		Dwarf player;

		int turn;

		int location[2];
		int earth_size[2];
		Earth* earth;

		int screen_status;
		// -1:	not yet defined
		// 0:	title screen
		// 1:	character screen
		// 2:	world view
		// 3:	battle view
		// 4:	building view

		std::string random_name();
		std::string data_dir, version, copyright;

		~World();
		void quit();

	private:
		void init_names();
		std::vector<std::string> firsts, lasts;

		void run();
		void move(int direction);
		void wait_turn();
		int enter_building(std::string building_name);
		void update();
		void die();
};


#endif
