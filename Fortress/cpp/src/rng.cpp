#include <random>

#include "rng.h"

RNG::RNG()
{
	std::random_device rd;
	mt_.seed(rd());
}

RNG::RNG(int seed)
{
	mt_.seed(seed);
}

double RNG::rand()
{
	std::uniform_real_distribution<double> distribution(0, 1);
	return distribution(mt_);
}

double RNG::rand(double max)
{
	std::uniform_real_distribution<double> distribution(0, max);
	return distribution(mt_);
}

double RNG::rand(double min, double max)
{
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(mt_);
}

int RNG::rand_int(int max)
{
	std::uniform_int_distribution<int> distribution(0, max);
	return distribution(mt_);
}

int RNG::rand_int(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mt_);
}

void RNG::seed(int seed)
{
	mt_.seed(seed);	
}

