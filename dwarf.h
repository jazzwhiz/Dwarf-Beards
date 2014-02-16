#include "task.h"

#ifndef DWARF_H
#define DWARF_H

class dwarf
{
	private:
		int beard_inc;
		int idle_time;
	public:
		dwarf(int loc[3]);
		void update();
		int beard;
		const char* name;
		task current;
		
		
};
 
#endif
