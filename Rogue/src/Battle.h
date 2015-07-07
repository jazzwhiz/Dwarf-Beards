#ifndef Battle_H
#define Battle_H

#include <vector>

class Monster;
class Dwarf;

class Battle
{
	public:
		Battle() {};
		Battle(std::vector<Monster>* monsters);
		Battle(Dwarf* player, std::vector<Monster>* monsters);
};

#endif
