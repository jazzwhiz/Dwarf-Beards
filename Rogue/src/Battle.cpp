#include <vector>
#include <algorithm>

#include <iostream>

#include "Battle.h"
#include "Monster.h"
#include "Dwarf.h"
#include "Draw.h"
#include "Keyboard.h"
#include "World.h"
#include "Location.h"

#include "rng.h"

Monster_Battle::Monster_Battle(std::vector<Monster>* _monsters)
: monsters(*_monsters), turn_number(0)
{
	n_monsters = monsters.size();

	for (int i = 0; i < n_monsters; i++)
		exp.push_back(0);

	// number of turns in an off screen monster battle
	while (n_monsters > 1 and (turn_number < 5 or turn_number < n_monsters))
		turn();

	for (int i = 0; i < n_monsters; i++)
		monsters[i].gain_exp(exp[i]);
}

void Monster_Battle::turn()
{
	turn_number++;

	if (n_monsters < 2)
		return;

	int target;
	int *order = new int[n_monsters];
	for (int i = 0; i < n_monsters; i++)
		order[i] = i;
	rng.shuffle(order, n_monsters);

	// loop over attackers
	for (int i = 0; i < n_monsters; i++)
	{
		target = (rng.rand_int(1, n_monsters - 1) + order[i]) % n_monsters;
		this->fight(order[i], target);
	} // i, attackers
	delete[] order;
}

void Monster_Battle::fight(int attacker, int defender)
{
	// miss: no exp, no damage, no death
	if (rng.rand() < 0.5)
		return;

	// set exp for each
	exp[attacker] = std::max(exp[attacker], monsters[defender].stats[5]);
	exp[defender] = std::max(exp[defender], monsters[attacker].stats[5]);

	int attack_style = rng.rand_int(1); // 0 - regular, 1 - magic
	if (not monsters[defender].take_damage(monsters[attacker], attack_style)) // if defender dies
	{
		monsters.erase(monsters.begin() + defender);
		exp.erase(exp.begin() + defender);
		n_monsters--;
	}
}

int dwarf_battle(World* w)
{
	std::vector<Monster>& monsters = w->earth->locations[w->location[0]][w->location[1]].monsters;
	int turn_number = 0;
	int exp = 0;

	int n_monsters = monsters.size();

	for (int i = 0; i < n_monsters; i++)
		exp = std::max(exp, monsters[i].stats[5]);

	bool battling = true;
	int attack_style = -1;
	int attack_target = -1;
	int attack, target;
	double damage;
	std::vector<std::string> readout;
	std::string tmp;

	while (battling and n_monsters > 0)
	{
		draw::battle(w, attack_style, attack_target, readout);

		// -1 - not selected yet, 0 - regular, 1 - magic
		attack = keyboard::battle();

		// quit
		if (attack == 0)
		{
			battling = false;
			return 0;
		}
		if (attack <= 2)
			attack_style = attack - 1;
		if (attack == 3 and attack_style >= 0 and attack_target >= 0)
		{
			turn_number++;
			int *order = new int[n_monsters + 1];
			readout.clear();
			for (int i = 0; i < n_monsters + 1; i++)
				order[i] = i;
			rng.shuffle(order, n_monsters + 1);

// TODO fix indexing in the following loop after death
			for (int i = 0; i < n_monsters + 1; i++)
			{
				if (order[i] == n_monsters) // dwarf is attacking
				{
					damage = monsters[attack_target].take_damage(w->player, attack_style);
					tmp = w->player.name;
					if (damage == 0)
						tmp += " misses ";
					else if (damage == -1)
					{
						tmp += " kills ";
						attack_target = -1; // you kill your target
					}
					else
						tmp += " does " + std::to_string((int)damage) + " damage to ";
					tmp += monsters[attack_target].name + "(" + std::to_string(monsters[attack_target].lvl) + ")";
				}
				else
				{
					target = (rng.rand_int(1, n_monsters) + order[i]) % (n_monsters + 1);
					if (target == n_monsters) // if targeting player
						damage = w->player.take_damage(monsters[order[i]], rng.rand_int(1));
					else
						damage = monsters[target].take_damage(monsters[order[i]], rng.rand_int(1));
					tmp = monsters[order[i]].name + "(" + std::to_string(monsters[order[i]].lvl) + ")";
					if (damage == 0)
						tmp += " misses ";
					else if (damage == -1)
					{
						tmp += " kills ";
						if (target == attack_target)
							attack_target = -1; // somebody kills your target
					}
					else
						tmp += " does " + std::to_string((int)damage) + " damage to ";
					if (target == n_monsters) // if targeting player
						tmp += w->player.name;
					else
						tmp += monsters[target].name + "(" + std::to_string(monsters[target].lvl) + ")";
				}
				readout.push_back(tmp);
				tmp.clear();

				// erase dead monsters
				for (uint j = 0; j < monsters.size(); j++)
				{
					if (monsters[j].hp <= 0)
					{
						monsters.erase(monsters.begin() + j);
						if (attack_target != -1 and j < (uint)attack_target)
							attack_target--;
					}
				} // j, monsters
			} // i, monsters
			n_monsters = monsters.size();

//			attack_target = -1;
			delete[] order;
		}
		if (attack >= 4)
		{
			attack_target = attack - 4;
			if (attack_target >= n_monsters)
				attack_target = -1;
		}

	}


	w->player.gain_exp(exp);
	readout.push_back("");
	readout.push_back("Victory!");
	readout.push_back(std::to_string(exp) + " experience gained.");

	// todo: level up?

	draw::battle(w, attack_style, attack_target, readout);
	return keyboard::simple();
}

