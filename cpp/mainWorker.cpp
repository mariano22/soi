#include <bits/stdc++.h>
#include "globalFiles.h"
#include "fdManage.h"
#include "localFiles.h"
#include "openedFiles.h"
#include "tokenControl.h"
#include "workerScope.h"
#include "task.h"
#include "ids.h"
#include "procTask.h"
#include "mainWorker.h"
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

syncQueues<task> *workerQueues = NULL;
localConections *workerConections = NULL;

void procToken(WorkerScope* myScope) {
	assert(false); /* TODO */
}

void* mainWorker(void *pid) { 
	int id = *(int*)pid;
	WorkerScope *myScope = new WorkerScope(id);
	myScope->MysyncQueues = &workerQueues[id];
	myScope->MylocalConections = &workerConections[id];
	
	if (!id) myScope->MytokenControl.recvT(token());
	while(true) {
		task t;
		myScope->MysyncQueues->recv(t,myScope->MytokenControl.tickTime());
		procTask(myScope,t);
		if (myScope->MytokenControl.mustProc()) {
			procToken(myScope);
		}
	}
	delete myScope;
	return NULL;
}

