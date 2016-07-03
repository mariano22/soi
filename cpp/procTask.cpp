#include <bits/stdc++.h>
#include <string>
#include "procTask.h"

using namespace std;
#define forall(it,v) for(typeof(v.begin()) it=v.begin();it!=v.end();++it)

#define dprint(v) cerr << #v"=" << v << endl //;)
#define forr(i,a,b) for(int i=(a); i<(b); i++)
#define forn(i,n) forr(i,0,n)
#define dforn(i,n) for(int i=n-1; i>=0; i--)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int,int> ii;

const int MAXN=100100;
int n;

void caseUserCon(WorkerScope *who,task& t){
				ClientId c = t.getCliente();
				responderCliente(c,mensaje::coneccionEstablecida(c),who);
}

void caseUserLsd(WorkerScope *who,task& t){
			globalFiles gl = globalFiles();
			mensaje l = mensaje::mOk();
			vector<string> arch = gl.archivosActuales();
			forall (it, arch ){ l.addArg(*it);  };
			ClientId cID = t.getCliente();
			responderCliente(cID,l, who);
}

void caseUserCreate(WorkerScope *who,task& t){
	string name = t.getFileName();
	ClientId cID = t.getCliente();
	WorkerId wID = who->MyglobalFiles.getOwner(name);
	if (noWorkerId!=wID){
		responderCliente(cID,mensaje::archivoExistente(),who);
	}else{
		if (who->MytokenQueues.isInCreate(name)) {
			responderCliente(cID,mensaje::archivoExistente(),who);
		}else{
			who->MytokenQueues.newCreate(name, cID);
		};
	}; 
}
void caseUserOpenRead(WorkerScope *who,task& t){
	string name = t.getFileName();
	ClientId cID = t.getCliente();
	GlobalId gID = idsManage::makeIdGlobal(who->MyIdsManage.myId(), cID);
	WorkerId wID = who->MyglobalFiles.getOwner(name);
	if (noWorkerId==wID){
		responderCliente(cID,mensaje::archivoNoExiste(),who);
	}else{
		task t2 = task::crear_workerOpenRead(name,gID);
		enviarWorker(wID,t2);
	}; 
}

void caseUserOpenWrite(WorkerScope *who,task& t){
	string name = t.getFileName();
	ClientId cID = t.getCliente();
	GlobalId gID = idsManage::makeIdGlobal(who->MyIdsManage.myId(), cID);
	WorkerId wID = who->MyglobalFiles.getOwner(name);
	if (noWorkerId==wID){
		responderCliente(cID,mensaje::archivoNoExiste(),who);
	}else{
		task t2 = task::crear_workerOpenWrite(name,gID);
		enviarWorker(wID,t2);
	}; 
}

void caseUserDelete(WorkerScope *who,task& t){
	string name  = t.getFileName();
	ClientId cID = t.getCliente();
	GlobalId gID = idsManage::makeIdGlobal(who->MyIdsManage.myId(), cID);
	WorkerId wID = who->MyglobalFiles.getOwner(name);
	if (noWorkerId==wID){
		responderCliente(cID,mensaje::archivoNoExiste(),who);
	}else{
		task t2 = task::crear_workerDelete(name,gID);
		enviarWorker(wID,t2);
	}; 
}

void caseUserBye(WorkerScope *who,task& t){
	ClientId cID = t.getCliente();
	vector<GlobalFd> gFds = who->MyopenedFiles.globalFdList(cID);
	responderCliente(cID,mensaje::mOk(),who);
	
	
	
	//	task t2 = task::crear_workerDelete(name,gID);
		//enviarWorker(wID,t2);
}
	
	
void procTask(WorkerScope *who,task& t) {
	switch (t.getTaskName()) {
		case userCon:
			caseUserCon(who,t); 
		break;
		case userLsd:
			caseUserLsd(who,t);
		break;
		case userCreate:
			caseUserCreate(who,t);
		break;
		case userOpenRead:
			caseUserOpenRead(who,t);
		break;
		case userOpenWrite:
			caseUserOpenWrite(who,t);
		break;
		case userDelete:
			caseUserDelete(who,t);
		break;
		case userWrite:
		break;
		case userRead:
		break;
		case userClose:
		break;
		case userBye:
			caseUserBye(who,t);
		break;
		case workerDelete:
		break;
		case workerOpenRead:
		break;
		case workerWrite:
		break;
		case workerRead:
		break;
		case workerOpenWrite:
		break;
		case workerSay:
		break;
		case workerOpenSucc:
		break;
		case workerClose:
		break;
		case workerCloseBye:
		break;
		case workerCloseSucc:
		break;
		case workerToken:
		break;
	}
}


int main() {
    
    return 0;
}
