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

		void update();
		void update_global(); // updates global things like evil and weather
		void update_local(); // updates local things within a location (monsters, trees, plants)

		void live_monster_data();
};

#endif
