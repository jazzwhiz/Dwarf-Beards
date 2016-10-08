#ifndef Modification_H
#define Modification_H

#include <string>

class Dwarf;

class Modification_Base
{
	public:
		Modification_Base() {};
		virtual void modify(Dwarf* player) = 0;
		int cost;
		std::string description;
};

class Drink : public Modification_Base
{
	public:
		Drink();
		void modify(Dwarf* player);
};

class Sleep : public Modification_Base
{
	public:
		Sleep();
		void modify(Dwarf* player);
};

#endif
