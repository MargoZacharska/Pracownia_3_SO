#ifndef __ConcurrentResource_Queue__
#define __ConcurrentResource_Queue__

#include <queue>
#include "pthread.h"


class ConcurrentResource_Queue
{
private:

	pthread_mutex_t	mutex;
	std::queue <int> Queue;

public:
	ConcurrentResource_Queue();
	~ConcurrentResource_Queue();

	void add(int value);
	int size();
	bool TakeOneIfExists();
	bool CheckIfExists();
	void CleanStale(int curreentDay);
};


#endif
