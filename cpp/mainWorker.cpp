#include <bits/stdc++.h>
#include "globalFiles.h"
#include "fdManage.h"
#include "localFiles.h"
#include "openedFiles.h"
#include "tokenControl.h"
#include "workerDirs.h"
#include "workerScope.h"
#include "task.h"
using namespace std;
#define dprint(v) cerr << #v"=" << v << endl //;)
#define forr(i,a,b) for(int i=(a); i<(b); i++)
#define forn(i,n) forr(i,0,n)
#define dforn(i,n) for(int i=n-1; i>=0; i--)
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int,int> ii;


workerDirs wList;
syncQueues<task> *workerQueues;

void* mainWorker(void *pid) { 
	int id = *(int*)pid;
	WorkerScope *myScope = new WorkerScope;
	// setUpComunic(myScope);
	if (!id) MytokenControl.recvT(token());
	while(true) {
		task *t;
		myScope->MysyncQueues.recv(t,MytokenControl.tickTime());
		// procTask(t);
		if (MytokenControl.mustProc()) {
			// procesar token
		}
	}
	return NULL;
}

int main() {
    wList.init("workerlist");
    pthread_t *wfs = (pthread_t *) malloc(wList.wlen()*sizeof(pthread_t));
    int *ids = new int[wList.wlen()];
    workerQueues = new syncQueues<task>[wList.wlen()];
    for(int i=0 ; i < wList.wlen() ; i++) {
		ids[i]=i;
		pthread_create(&wfs[i], 0, mainWorker, &ids[i]); 
	}
	for(int i=0 ; i < wList.wlen() ; i++) 
		pthread_join(wfs[i],NULL);
		
	delete[] ids;
	delete[] workerQueues;
	free(wfs);
    
    return 0;
}
