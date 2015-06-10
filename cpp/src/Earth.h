#ifndef EARTH_H
#define EARTH_H

#include "Location.h"

class Earth
{
	public:
		Earth() {};
		
		Location earth[50][50][11];
		void init();
		void fill_earth();
		void make_vein(int lid, int size);
		void make_cluster(int lid, int size);
};
#endif
