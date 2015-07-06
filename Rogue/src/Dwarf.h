#ifndef Dwarf_H
#define Dwarf_H

#include <string>

class Dwarf
{
	public:
		Dwarf() {};
		Dwarf(std::string name);
		std::string name;
		int beard, exp;
		double thirst, sleepiness;

};

#endif
