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

		std::string random_name();
		std::string data_dir, version, copyright;

		~World();
		void quit();

	private:
		void init_names();
		std::vector<std::string> firsts, lasts;

		void run();
};


#endif
