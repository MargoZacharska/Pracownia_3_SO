

#include "sched.h"
#include "pthread.h"

using namespace std;

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
	bool TakeOneIfExists();
	bool CheckIfExists();
	
};
