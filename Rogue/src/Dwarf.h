#ifndef Dwarf_H
#define Dwarf_H

#include <string>

class Dwarf
{
	public:
		Dwarf() {};
		Dwarf(std::string name);
		std::string name;
		int beard;
		int exp;
};

#endif
