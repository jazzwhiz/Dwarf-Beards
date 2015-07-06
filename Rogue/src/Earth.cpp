
#include <iostream>

#include "Earth.h"
#include "Location.h"

Earth::Earth()
{
	allocated = false;
}

Earth::Earth(int max_x, int max_y)
{
	allocated = false;
	earth_size[0] = max_x;
	earth_size[1] = max_y;
	locations = new Location*[earth_size[0]];
	for (int x = 0; x < earth_size[0]; x++)
		locations[x] = new Location[earth_size[1]];
	allocated = true;
}

Earth::~Earth()
{
	if (allocated)
	{
		for (int x = 0; x < earth_size[0]; x++)
			delete [] locations[x];
		delete [] locations;
		allocated = false;
	}
}

