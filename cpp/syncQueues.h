#ifndef SYNCQUEUES_H
#define SYNCQUEUES_H

#include <iostream>
#include <queue>
#include <unistd.h>
#include <time.h>
using namespace std;
/* 
    Este módulo contiene implementadas colas que pueden ser compartidas por procesos, con métodos para pushear y popear elementos cuidando las condiciones de carrera, valiéndose del uso de locks y variables de condición de POSIX.
*/
template<class T>
class syncQueues {
	public:
	syncQueues();
    // Inserta un elemento en la cola
	void push(T e); 
    // Extrae un elemento en la cola. Si la cola está vacía, duerme hasta que algún otro proceso le inserte un elemento o hasta que se cumpla el timeout especificado. Devuelve false si salió por timeout y true si se efectivizó el recV.
	bool recv(T& rv, int miliSeconds); 
	private:
	queue<T> cola;
	pthread_mutex_t lock;
	pthread_cond_t condvar;
};


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
	if (miliSeconds>0) {
		struct timespec timevar;
		clock_gettime(CLOCK_REALTIME, &timevar);
		timevar.tv_nsec += ((miliSeconds%1000)*1000000);
		timevar.tv_nsec %= 1000000000;
		timevar.tv_sec += miliSeconds/1000;
	
		if(!cola.size()) pthread_cond_timedwait(&condvar, &lock,&timevar);
	} else {
		while(!cola.size()) pthread_cond_wait(&condvar, &lock);
	}
	
	bool rflag = cola.size();
	if (rflag) { 
		rv = cola.front(); 
		cola.pop(); 
	}
	pthread_mutex_unlock(&lock);
	return rflag; 
}

#endif
