#ifndef TASK_H
#define TASK_H
class Task
{
	public:
		const char* name;
		void move();
};
#endif

#ifndef IDLE_H
#define IDLE_H
class Idle:Task
{
	public:
		const char* name;
		Idle(int earth,int start[3],int wait);
};
#endif
