#ifndef Dwarf_H
#define Dwarf_H

#include <string>

#include "Being.h"

extern std::string stat_names[5];

class Dwarf : public Being
{
	public:
		Dwarf() {};
		Dwarf(std::string name);
		int gold;
		double thirst, sleepiness;

		void gain_exp(int _exp);
		void update();
};

#endif
