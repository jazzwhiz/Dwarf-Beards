#include <vector>
#include <algorithm>

#include <iostream>

#include "Battle.h"
#include "Monster.h"
#include "Dwarf.h"

#include "rng.h"

Monster_Battle::Monster_Battle(std::vector<Monster>* _monsters)
: monsters(*_monsters), turn_number(0)
{
	n_monsters = monsters.size();

	for (int i = 0; i < n_monsters; i++)
		exp.push_back(0);

	// number of turns in an off screen monster battle
	while (turn_number < 3 and n_monsters > 1)
		this->turn();

	for (int i = 0; i < n_monsters; i++)
		// monsters don't gain as much experience
		monsters[i].gain_exp(exp[i]);
}

void Monster_Battle::turn()
{
	turn_number++;

	if (n_monsters < 2)
		return;

	int target, order[n_monsters];
	for (int i = 0; i < n_monsters; i++)
		order[i] = i;
	rng.shuffle(order, n_monsters);

	// loop over attackers
	for (int i = 0; i < n_monsters; i++)
	{
		target = (rng.rand_int(1, n_monsters - 1) + i) % n_monsters;
		this->fight(i, target);
	} // i, attackers
}

void Monster_Battle::fight(int attacker, int defender)
{
	// miss: no exp, no damage, no death
	if (rng.rand() < 0.5)
		return;

	int attack_style, damage;

	// set exp for each
	exp[attacker] = std::max(exp[attacker], monsters[defender].stats[5]);
	exp[defender] = std::max(exp[defender], monsters[attacker].stats[5]);

	attack_style = rng.rand_int(1);
	// do at least one damage per hit
	damage = std::max(monsters[attacker].stats[attack_style + 1] - monsters[defender].stats[attack_style + 3], 1);
	if (not monsters[defender].damage(damage)) // if defender dies
	{
		monsters.erase(monsters.begin() + defender);
		exp.erase(exp.begin() + defender);
		n_monsters--;
	}
}

Dwarf_Battle::Dwarf_Battle(Dwarf& player, std::vector<Monster>& monsters)
: monsters(monsters), turn_number(0)
{

}

