#ifndef Weather_H
#define Weather_H

#include <string>

class Weather
{
	public:
		Weather();
		std::string temp();
		std::string precip();
		void update();

	private:
		double temperature; // Faranheit because duh
		double precipitation; // 0 - 10
};

#endif
