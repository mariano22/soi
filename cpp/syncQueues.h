#ifndef SYNCQUEUES_H
#define SYNCQUEUES_H

#include <iostream>
#include <queue>
#include <unistd.h>
#include <time.h>
using namespace std;

template<class T>
class syncQueues {
	public:
	syncQueues();
	void push(T e);
	bool recv(T* rv, int miliSeconds);
	private:
	queue<T> cola;
	pthread_mutex_t lock;
	pthread_cond_t condvar;
};

#endif
