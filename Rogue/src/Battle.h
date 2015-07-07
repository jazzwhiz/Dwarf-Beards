#ifndef Battle_H
#define Battle_H

#include <vector>

class Monster;
class Dwarf;

class Monster_Battle
{
	public:
//		Monster_Battle();
		Monster_Battle(std::vector<Monster>* monsters);

	private:
		std::vector<Monster>& monsters;
		int n_monsters; // number of monsters
		int turn_number; // battle turn number
		std::vector<int> exp; // amount of experience awarded to survivors

		void turn();
		void fight(int attacker, int defender);

};

class Dwarf_Battle
{
	public:
		Dwarf_Battle();
		Dwarf_Battle(Dwarf& player, std::vector<Monster>& monsters);

	private:
		std::vector<Monster>& monsters;
		int n_monsters; // number of monsters
		int turn_number; // battle turn number

		void turn();
};


#endif
