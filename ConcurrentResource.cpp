#include "stdio.h"
#include "ConcurrentResource.h"

ConcurrentResource::ConcurrentResource()
{
	resource = 0;
	pthread_mutex_init(&mutex, NULL);
}

ConcurrentResource::~ConcurrentResource()
{
	pthread_mutex_destroy(&mutex);
}

void ConcurrentResource::add(int x)
{
	pthread_mutex_lock(&mutex);
	resource += x;
	pthread_mutex_unlock(&mutex);
}

int ConcurrentResource::get()
{
	int value;
	pthread_mutex_lock(&mutex);
	value = resource;
	pthread_mutex_unlock(&mutex);
	return value;
}


bool ConcurrentResource::TakeOneIfExists()
{
	bool result = false;
	pthread_mutex_lock(&mutex);
	if (resource > 0)
	{
		resource--;
		result = true;
	}
	pthread_mutex_unlock(&mutex);
	return result;
}


bool ConcurrentResource::CheckIfExists(){
	bool result = false;
	pthread_mutex_lock(&mutex);
	if (resource > 0){
		result = true;
	}
	pthread_mutex_unlock(&mutex);
	return result;
}

int ConcurrentResource::clean(){
	return resource = 0;
}
