#include <iostream>
#include <queue>
#include <unistd.h>
#include <time.h>
using namespace std;

template<class T>
class syncQueues {
	public:
	syncQueues() {
		lock = PTHREAD_MUTEX_INITIALIZER;
		condvar = PTHREAD_COND_INITIALIZER;
	}
	void push(T e) {
		pthread_mutex_lock(&lock);
		cola.push(e);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&condvar);
	}
	bool recv(T* rv, int miliSeconds) {
		pthread_mutex_lock(&lock);
		
		struct timespec timevar;
		clock_gettime(CLOCK_REALTIME, &timevar);
		timevar.tv_nsec += ((miliSeconds%1000)*1000000);
		timevar.tv_nsec %= 1000000000;
		timevar.tv_sec += miliSeconds/1000;
		
		pthread_cond_timedwait(&condvar, &lock,&timevar);
		
		bool rflag = cola.size();
		if (rflag) { 
			*rv = cola.front(); 
			cola.pop(); 
		}
		pthread_mutex_unlock(&lock);
		return rflag; 
	}
	private:
	queue<T> cola;
	pthread_mutex_t lock;
	pthread_cond_t condvar;
};

