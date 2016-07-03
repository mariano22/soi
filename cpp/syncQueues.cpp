#include "syncQueues.h"

template<class T>
syncQueues<T>::syncQueues() {
	lock = PTHREAD_MUTEX_INITIALIZER;
	condvar = PTHREAD_COND_INITIALIZER;
}
template<class T>
void syncQueues<T>::push(T e) {
	pthread_mutex_lock(&lock);
	cola.push(e);
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&condvar);
}
template<class T>
bool syncQueues<T>::recv(T& rv, int miliSeconds) {
	pthread_mutex_lock(&lock);
	
	struct timespec timevar;
	clock_gettime(CLOCK_REALTIME, &timevar);
	timevar.tv_nsec += ((miliSeconds%1000)*1000000);
	timevar.tv_nsec %= 1000000000;
	timevar.tv_sec += miliSeconds/1000;
	
	pthread_cond_timedwait(&condvar, &lock,&timevar);
	
	bool rflag = cola.size();
	if (rflag) { 
		rv = cola.front(); 
		cola.pop(); 
	}
	pthread_mutex_unlock(&lock);
	return rflag; 
}

