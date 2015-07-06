#ifndef Earth_H
#define Earth_H

class Location;

class Earth
{
	public:
		Earth();
		Earth(int max_x, int max_y);
		~Earth();

		bool allocated;

		int earth_size[2];
		Location** locations;
};

#endif
