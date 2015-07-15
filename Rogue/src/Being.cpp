#include <algorithm>

#include <iostream>

#include "Being.h"

#include "rng.h"

double Being::take_damage(Being attacker, int attack_style)
{
	// 20% chance of miss
	if (rng.rand() < 0.2)
		return 0;

	double damage;
	damage = attacker.stats[attack_style + 1] - stats[attack_style + 3];
	damage = rng.gaussian(damage, 0.2 * damage); // 20% spread in damage
	damage = std::max(damage, 1.); 	// do at least one damage per hit

	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		return -1;
		// todo: die
	}
	return damage;
}

