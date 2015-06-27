#ifndef TASK_H
#define TASK_H

#include <string>

class Task
{
	public:
		virtual void update(int (*loc)[3]) = 0;
		std::string name;
};

class Idle : public Task
{
	public:
		Idle(int idle_time);
		Idle() {};

		void update(int (*loc)[3]);
		std::string name;
	private:
		bool initial_loc;

		int idle_sign; // -1, +1
		int idle_index; // 0, 1 - only idle horizontally

		void step(int (*loc)[3]);
};

#endif
