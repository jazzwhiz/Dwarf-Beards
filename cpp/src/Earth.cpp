#include <iostream>

#include "Earth.h"
#include "Location.h"
#include "rng.h"

void Earth::init()
{
	int x, y, z;
	for (x = 0; x < 50; x++)
	{
		for (y = 0; y < 50; y++)
		{
			earth[x][y][0] = Soil(); // top layer all soil, floor
			earth[x][y][0].floor = true;
			for (z = 1; z<11; z++)
			{
				earth[x][y][z] = Bedrock(); // beneath starts out as bedrock
			}
		}
	}
	fill_earth();
}
void Earth::fill_earth()
{
	for (int lid = 2; lid <= 4; lid++)
	{
		for (int num_veins = 0; num_veins < rng.rand_int(1, 4); num_veins++)
			make_vein(lid, rng.rand_int(25, 100));
		for (int num_clusters = 0; num_clusters < rng.rand_int(1, 4); num_clusters++)
			make_cluster(lid, rng.rand_int(25, 100));
	}
}
void Earth::make_vein(int lid, int size)
{
	std::cout << "Filling a vein of size " << size << " with " << Location_from_lid(lid).name << std::endl;

	int current[3] = {rng.rand_int(49), rng.rand_int(49), rng.rand_int(1, 10)}; // don't start in soil
	int direction[3] = {rng.rand_int(-2, 2), rng.rand_int(-2, 2), rng.rand_int(-2, 2)}; // vector direction to move sometimes
	int direction_abs_sum = 0;
	for (int i = 0; i < 3; i++)
		direction_abs_sum += std::abs(direction[i]);
	int directions[direction_abs_sum][3]; // possible directions to step
	// initialize all to zero
	for (int i = 0; i < direction_abs_sum; i++)
	{
		for (int j = 0; j < 3; j++)
			directions[i][j] = 0;
	}
	// fill in with allowable directions to step
	int i = 0; // 0, 1, 2
	int j = 0;
	while (j < direction_abs_sum)
	{
		if (direction[i] == 0)
			i++;
		if (directions[i] > 0)
		{
			direction[i]--;
			directions[j][i]++;
			j++;			
		}
	}
	int direction_num; // which of these directions will be selected

	int N = 0; // number changed thus far

	earth[current[0]][current[1]][current[2]] = Location_from_lid(lid);
	N++;
	while (N < size)
	{
		if (rng.rand() > 0.5) // random direction
			current[rng.rand_int(2)] += 2* rng.rand_int(1) - 1; // either +1 or -1
		else // vein direction
		{
			direction_num = rng.rand_int(direction_abs_sum - 1);
			for (int i = 0; i < 3; i++)
				current[i] += directions[direction_num][i];
		}

		if (current[0] >= 50)
			current[0]--;
		if (current[1] >= 50)
			current[1]--;
		if (current[2] >= 11)
			current[2]--;
		if (current[0] < 0)
			current[0]++;
		if (current[1] < 0)
			current[1]++;
		if (current[2] < 0)
			current[2]++;

		if (earth[current[0]][current[1]][current[2]].lid != lid)
		{
			earth[current[0]][current[1]][current[2]] = Location_from_lid(lid);
			N++;
		}
	}
}
void Earth::make_cluster(int lid, int size)
{
	std::cout << "Filling a cluster of size " << size << " with " << Location_from_lid(lid).name << std::endl;

	int current[3] = {rng.rand_int(49), rng.rand_int(49), rng.rand_int(1, 10)}; // don't start in soil

	int N = 0; // number changed thus far

	earth[current[0]][current[1]][current[2]] = Location_from_lid(lid);
	N++;
	while (N < size)
	{
		current[rng.rand_int(2)] += 2* rng.rand_int(1) - 1; // either +1 or -1

		if (current[0] >= 50)
			current[0]--;
		if (current[1] >= 50)
			current[1]--;
		if (current[2] >= 11)
			current[2]--;
		if (current[0] < 0)
			current[0]++;
		if (current[1] < 0)
			current[1]++;
		if (current[2] < 0)
			current[2]++;

		if (earth[current[0]][current[1]][current[2]].lid != lid)
		{
			earth[current[0]][current[1]][current[2]] = Location_from_lid(lid);
			N++;
		}
	}
}

