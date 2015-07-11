#include <vector>
#include <algorithm>

#include <iostream>

#include "Battle.h"
#include "Monster.h"
#include "Dwarf.h"
#include "Draw.h"
#include "World.h"

#include "rng.h"

Monster_Battle::Monster_Battle(std::vector<Monster>* _monsters)
: monsters(*_monsters), turn_number(0)
{
	n_monsters = monsters.size();

	for (int i = 0; i < n_monsters; i++)
		exp.push_back(0);

	// number of turns in an off screen monster battle
	while (turn_number < 5 and n_monsters > 1)
		this->turn();

	for (int i = 0; i < n_monsters; i++)
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

	attack_style = rng.rand_int(1); // 0 - regular, 1 - magic
	// do at least one damage per hit
	damage = std::max(monsters[attacker].stats[attack_style + 1] - monsters[defender].stats[attack_style + 3], 1);
	if (not monsters[defender].damage(damage)) // if defender dies
	{
		monsters.erase(monsters.begin() + defender);
		exp.erase(exp.begin() + defender);
		n_monsters--;
	}
}

Dwarf_Battle::Dwarf_Battle(Dwarf& player, std::vector<Monster>* _monsters, World* w)
: monsters(*_monsters), turn_number(0), exp(0)
{
	n_monsters = monsters.size();

	for (int i = 0; i < n_monsters; i++)
		exp = std::max(exp, monsters[i].stats[5]);

	while (n_monsters > 0)
	{
		draw::battle(w);
	}

	player.gain_exp(exp);

	// todo
}

bool dwarf_attack(Dwarf* player, Monster* monster, int attack_style)
{
	double damage = std::max(player->stats[attack_style + 1] - monster->stats[attack_style + 3], 1);
	return monster->damage(damage); // true if monster is still alive
}
bool dwarf_defend(Dwarf* player, Monster* monster, int attack_style)
{
	double damage = std::max(monster->stats[attack_style + 1] - player->stats[attack_style + 3], 1);
	return player->damage(damage); // true if dwarf is still alive
}

