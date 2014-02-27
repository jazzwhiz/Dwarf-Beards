#ifndef DWARF_H
#define DWARF_H

#include "Task.h"
#include "World.h"

class Dwarf
{
	private:
		int beard_inc;
		int idle_time;
		const char* tmp;
	public:
		Dwarf(const char* name,int loc[3]);
		void update();
		int beard;
		const char* name;
		Task current;
		
		
};
#endif
