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
int workerCant;
    
void procToken(WorkerScope* myScope) {
	token Token = myScope->MytokenControl.getT();
	vector< pair<string,WorkerId> > ListaAltasOld = Token.getListaAltas();
	vector< pair<string,WorkerId> > ListaBajasOld = Token.getListaBajas();

	vector< pair<string,WorkerId> > ListaAltas, ListaBajas;
	forall(it, ListaAltasOld) if ( it->second != myScope->MyIdsManage.myId() ) ListaAltas.push_back(*it);
	forall(it, ListaBajasOld) if ( it->second != myScope->MyIdsManage.myId() ) ListaBajas.push_back(*it);
	
	vector< pair<string,WorkerId> > ListaBajasNew = ListaBajas;
	vector< string > MyDeletes = myScope->MytokenQueues.getDeletes();
	forall(it,MyDeletes) ListaBajasNew.push_back( pair<string,WorkerId>(*it, myScope->MyIdsManage.myId()) );
	vector< pair<string,ClientId> >  MyCreateRequests = myScope->MytokenQueues.getCreates();
	
	vector< pair<string,ClientId> >  MyCreateRequestsOk;
	forall(it, MyCreateRequests) {
		bool f_is = false;
		forall(jt,ListaAltas) f_is = f_is || jt->first == it->first;
		if ( f_is ) 
			responderCliente(it->second, mensaje::archivoExistente(), myScope);
		else {
			responderCliente(it->second, mensaje::mOk(), myScope);
			myScope->MylocalFiles.create(it->first);
			createf(it->first);
			MyCreateRequestsOk.push_back(*it);
		}
	}
	
	vector< pair<string,WorkerId> > ListaAltasNew = ListaAltas;
	forall(it,MyCreateRequestsOk) 
		ListaAltasNew.push_back( pair<string,WorkerId>(it->first, myScope->MyIdsManage.myId()) );

    forall(it,ListaAltasNew) 
		myScope->MyglobalFiles.alta(it->first, it->second);
	forall(it,ListaBajasNew)
		myScope->MyglobalFiles.baja(it->first);

    token NewToken(ListaAltasNew, ListaBajasNew);
    task Orden = task::crear_workerToken(NewToken);
    enviarWorker( myScope->MyIdsManage.nextWorkerId(), Orden );
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

