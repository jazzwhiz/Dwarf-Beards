#include <string>

#include "Weather.h"

#include "rng.h"

Weather::Weather()
{
	temperature = rng.rand(35, 65);
	precipitation = rng.rand(4);
}
std::string Weather::temp()
{
	std::string r;
	double apparent_temperature = temperature + rng.rand(-3, 3);
	if (apparent_temperature < 0)
		r = "frigid";
	else if (apparent_temperature < 27)
		r = "cold";
	else if (apparent_temperature < 38)
		r = "chilly";
	else if (apparent_temperature < 44)
		r = "cool";
	else if (apparent_temperature < 56)
		r = "regular";
	else if (apparent_temperature < 63)
		r = "fine";
	else if (apparent_temperature < 74)
		r = "warm";
	else if (apparent_temperature < 87)
		r = "toasty";
	else if (apparent_temperature < 97)
		r = "hot";
	else
		r = "sweltering";

	return r;
}
std::string Weather::precip()
{
	std::string r;

	if (precipitation < 3)
		r = "clear";
	else if (precipitation < 6)
	{
		if (temperature > 36)
			r = "drizzly";
		else if (temperature > 31)
			r = "sleeting";
		else
			r = "flurries";
	}
	else if (precipitation < 8)
	{
		if (temperature > 36)
			r = "pouring";
		else if (temperature > 31)
			r = "freezing rain";
		else
			r = "snowing";
	}
	else
	{
		if (temperature > 36)
			r = "driving rain";
		else if (temperature > 31)
			r = "hailing";
		else
			r = "blizzard";
	}
	return r;
}
void Weather::update()
{
	// change temperature
	if (rng.rand() < 0.05)
		temperature += rng.rand(-2, 2);

	// check temperature edge cases
	if (temperature < -10)
		temperature += rng.rand(3);
	if (temperature > 110)
		temperature -= rng.rand(3);

	// change precipitation
	if (rng.rand() < 0.3) // stay the same most of the time
		precipitation += rng.rand(-1, 1);
	if (rng.rand() < 0.02)
		precipitation += rng.rand(-6, 6); // occasinally change a lot

	// check precipitation edge cases
	if (precipitation < 0)
		precipitation += rng.rand(2);
	if (precipitation > 10)
		precipitation -= rng.rand(2);
}
