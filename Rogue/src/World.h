#ifndef World_H
#define World_H

#include <string>
#include <vector>

#include "Dwarf.h"

class World
{
	public:
		World();

		Dwarf player;

		std::string random_name();
		std::string data_dir, version, copyright;

		void quit();

	private:
		void init_names();

		std::vector<std::string> firsts, lasts;

		void run();
};


#endif
