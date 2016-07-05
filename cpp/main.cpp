#include <bits/stdc++.h>
#include <pthread.h>
#include "globalFiles.h"
#include "fdManage.h"
#include "localFiles.h"
#include "openedFiles.h"
#include "tokenControl.h"
#include "workerScope.h"
#include "task.h"
#include "procTask.h"
#include "mainWorker.h"
#include "dispatcher.h"
#include "syncQueues.h"
using namespace std;

int main(int argc, char *argv[]) {
	if ( argc!=2 || sscanf(argv[1],"%d",&workerCant)<1 ) {
		cout << "Modo de uso:\n\tmain <cantidad de workers>\n";
		return 0;
	}
	
    pthread_t *wfs = (pthread_t *) malloc(workerCant*sizeof(pthread_t));
    int *ids = new int[workerCant];
    workerQueues = new syncQueues<task>[workerCant];
    workerConections = new localConections[workerCant];
    for(int i=0 ; i < workerCant ; i++) {
		ids[i]=i;
		pthread_create(&wfs[i], 0, mainWorker, &ids[i]); 
	}
	
	launchDispatcher();
	
	for(int i=0 ; i < workerCant ; i++) 
		pthread_join(wfs[i],NULL);
		
	delete[] ids;
	delete[] workerQueues;
	free(wfs);
    
    return 0;
}
