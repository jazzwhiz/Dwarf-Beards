#ifndef rng_H
#define rng_H

#include <random>

class RNG
{
	public:
		RNG();
		RNG(int seed);

		double rand();
		double rand(double max);
		double rand(double min, double max);
		int rand_int(int max);			// uniform 0 to max (inclusive)
		int rand_int(int min, int max);	// uniform min to max (inclusive)

		void seed(int seed);

	private:
		std::mt19937 mt_;
};

extern RNG rng; // THE rng
extern int seed;

#endif
