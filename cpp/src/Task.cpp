#include <string>

#include <iostream>

#include "Task.h"
#include "rng.h"
//#include "Astar.h"

Idle::Idle(int idle_time)
: name("Idle"), initial_loc(true)
{
	// need to check that the direction is valid
	idle_sign = 2 * rng.rand_int(1) - 1;
	idle_index = rng.rand_int(1);
}

void Idle::update(int (*loc)[3])
{
	step(loc);
}

void Idle::step(int (*loc)[3])
{
	if (initial_loc)
		(*loc)[idle_index] += idle_sign;
	else
		(*loc)[idle_index] -= idle_sign;
	initial_loc = not initial_loc;
}


