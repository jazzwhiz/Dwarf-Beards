#ifndef Building_H
#define Building_H

#include <string>
#include <vector>

class Dwarf;
class Modification_Base;

class Building_Base
{
	public:
		std::string name;
		int capacity;
		std::vector<Modification_Base*> modifications;
};

class Tavern : public Building_Base
{
	public:
		Tavern();
};

class Inn : public Building_Base
{
	public:
		Inn();
};

class Marketplace : public Building_Base
{
	public:
		Marketplace();
};

#endif
