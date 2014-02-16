#ifndef TASK_H
#define TASK_H
class task
{
	public:
		const char* name;
		void move();
};
#endif

#ifndef IDLE_H
#define IDLE_H
class idle:task
{
	public:
		const char* name;
		idle(int earth,int start[3],int wait);
};
#endif
