#ifndef Being_H
#define Being_H

#include <string>

class Being
{
	public:
		Being() {};

		std::string name;
		int stats[6]; // maxhp, atk, matk, def, mdef, exp (awarded)
		int lvl, exp;
		double hp;

		// attack_style: 0 - regular, 1 - magic
		// returns damage done: 0 - miss, -1 means target is killed
		double take_damage(Being attacker, int attack_style);
		void gain_exp(int _exp);
		void update();

};

#endif
