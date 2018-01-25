#ifndef __ConcurrentResource__
#define __ConcurrentResource__

#include "sched.h"
#include "pthread.h"


class ConcurrentResource
{
private:

	pthread_mutex_t	mutex;

public:

	ConcurrentResource();
	~ConcurrentResource();

	volatile int resource;
	void add(int x);
	int get();
	int clean();
	bool TakeOneIfExists();
	bool CheckIfExists();
};


#endif
