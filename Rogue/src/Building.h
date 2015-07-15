#ifndef Building_H
#define Building_H

#include <string>

class Dwarf;

class Building_Base
{
	public:
		std::string name;
		int capacity;
};

class Tavern : public Building_Base
{
	public:
		Tavern();
		int cost;
		void drink(Dwarf* player);
};

class Inn : public Building_Base
{
	public:
		Inn();
		int cost;
		void sleep(Dwarf* player);
};

class Marketplace : public Building_Base
{
	public:
		Marketplace();
};

#endif
