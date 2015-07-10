#ifndef Building_H
#define Building_H

#include <string>

class Building_Base
{
	public:
		std::string name;
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
