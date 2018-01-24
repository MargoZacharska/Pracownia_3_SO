#include "ConcurrentResource_Queue.h"


ConcurrentResource_Queue::ConcurrentResource_Queue()
{
	pthread_mutex_init(&mutex, NULL);
}

ConcurrentResource_Queue::~ConcurrentResource_Queue()
{
	pthread_mutex_destroy(&mutex);
}

void ConcurrentResource_Queue::add(int value)
{
	pthread_mutex_lock(&mutex);
	Queue.push(value);
	pthread_mutex_unlock(&mutex);
}

int ConcurrentResource_Queue::size()
{
	int s = 0;
	pthread_mutex_lock(&mutex);
	s = Queue.size();
	pthread_mutex_unlock(&mutex);
	return s;
}

bool ConcurrentResource_Queue::TakeOneIfExists()
{
	bool result = false;
	pthread_mutex_lock(&mutex);
	if (!Queue.empty())
	{
		Queue.pop();
		result = true;
	}
	pthread_mutex_unlock(&mutex);
	return result;
}

bool ConcurrentResource_Queue::CheckIfExists()
{
	pthread_mutex_lock(&mutex);
	bool result = !Queue.empty();
	pthread_mutex_unlock(&mutex);
	return result;
}

void ConcurrentResource_Queue::CleanStale(int curreentDay)
{
	while (!Queue.empty() && Queue.front() < curreentDay - 7)
	{
		Queue.pop();
	}
}
