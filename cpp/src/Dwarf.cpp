#include <string>

#include <iostream>

#include "Dwarf.h"
#include "Task.h"
#include "rng.h"

Dwarf::Dwarf(std::string name, int _loc[3])
: name(name), beard(0)
{
	for (int i = 0; i < 3; i++)
		loc[i] = _loc[i];

	idle_time = rng.rand_int(40, 200);

	current = new Idle(idle_time);
}

Dwarf::~Dwarf()
{
//	delete current;
}

void Dwarf::update()
{
	(*current).update(&loc);
}

