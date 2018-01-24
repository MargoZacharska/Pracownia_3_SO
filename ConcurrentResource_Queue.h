
#include <queue>
#include "pthread.h"

using namespace std;

class ConcurrentResource_Queue
{
private:

	pthread_mutex_t	mutex;
	queue <int> Queue;

public:
	ConcurrentResource_Queue();
	~ConcurrentResource_Queue();

	void add(int value);
	int size();
	bool TakeOneIfExists();
	bool CheckIfExists();
	void CleanStale(int curreentDay);
};


