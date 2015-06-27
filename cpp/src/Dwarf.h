#ifndef DWARF_H
#define DWARF_H

#include <string>

#include "Task.h"

class Dwarf
{
	private:
		int beard_inc;
		int idle_time;

	public:
		Dwarf(std::string name, int _loc[3]);
		~Dwarf();
		int loc[3];
		std::string name;
		int beard;
		void update();
		Task* current;
		
		
};
#endif
