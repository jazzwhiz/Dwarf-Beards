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
//		std::string name;
		int gold;
		double thirst, sleepiness;
//		int stats[5]; // maxhp, atk, matk, def, mdef
//		double hp;

		void gain_exp(int _exp);
		void update();
};

#endif
