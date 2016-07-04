#include <bits/stdc++.h>
#include <string>
#include "procTask.h"

using namespace std;
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)

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

void responderClienteRemoto( GlobalId Idg,mensaje Msj){
	WorkerId W = idsManage::globalIdToWorker(Idg);
	ClientId C = idsManage::globalIdToClient(Idg);
	task T = task::crear_workerSay(C, Msj);
	enviarWorker(W,T);
}

void caseUserCon(WorkerScope *who,task& t){
				ClientId c = t.getCliente();
				responderCliente(c,mensaje::coneccionEstablecida(c),who);
}

void caseUserLsd(WorkerScope *who,task& t){
			mensaje l = mensaje::mOk();
			vector<string> arch = who->MyglobalFiles.archivosActuales();
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
	WorkerId w;
	task order;
	forall(it,gFds){
			order=task::crear_workerCloseBye(*it);
			w=idsManage::globalFdToWorker(*it);
			enviarWorker(w,order);
			who->MyopenedFiles.registerClose(*it);
	}
}

void caseUserWrite(WorkerScope *who,task& t){
	ClientId cID = t.getCliente();
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = idsManage::makeIdGlobal(who->MyIdsManage.myId(),cID);
	vector<GlobalFd> gFdS = (who->MyopenedFiles.globalFdList(cID));

	if ( find(gFdS.begin(), gFdS.end(), gFd) != gFdS.end() ){
		int sizeF = t.getSizeTxt();
		int sizeA = (t.getStrTxt()).size();
		string txt;
		if(sizeF<sizeA){
			txt = t.getStrTxt(); 
		}else{
			txt = t.getStrTxt().substr(0,sizeF);
		}
		WorkerId w = idsManage::globalFdToWorker(gFd);
		task order = task::crear_workerWrite(txt,gFd, idG);
		comunic:enviarWorker(w,order);
	}else{
		responderCliente(cID, mensaje::permisoDenegado(), who);
	}
}
	

void caseUserRead(WorkerScope *who,task& t){
	ClientId cID = t.getCliente();
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = idsManage::makeIdGlobal(who->MyIdsManage.myId(),cID);
	int sizeF = t.getSizeTxt();
	bool b=false;
	forall(it, (who->MyopenedFiles.globalFdList(cID))){ 
		b = b || (*it==gFd);
	}
	string txt;
	if (b){
		WorkerId w = idsManage::globalFdToWorker(gFd);
		task order = task::crear_workerRead(sizeF,gFd, idG);
		comunic:enviarWorker(w,order);
	}else{
		responderCliente(cID, mensaje::permisoDenegado(), who);
	}
}

void caseUserClose(WorkerScope *who,task& t){
	ClientId cID = t.getCliente();
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = idsManage::makeIdGlobal(who->MyIdsManage.myId(),cID);
	vector<GlobalFd> gFdS = (who->MyopenedFiles.globalFdList(cID));
	
	if ( find(gFdS.begin(), gFdS.end(), gFd) != gFdS.end() ){
		WorkerId w = idsManage::globalFdToWorker(gFd);
		task order = task::crear_workerClose(gFd, idG);
		comunic:enviarWorker(w,order);
	}else{
		responderCliente(cID, mensaje::permisoDenegado(), who);
	}
}

void caseWorkerDelete(WorkerScope *who,task& t){
	string name  = t.getFileName();
	GlobalId idG = t.getIdGlobal();
	fileStatus state = who->MylocalFiles.status(name);
	if (state == noFile){
		responderClienteRemoto(idG,mensaje::archivoNoExiste());
	}else if (state == unused){
		who->MytokenQueues.newDelete(name);
		responderClienteRemoto(idG, mensaje::archivoBorrado());
		who->MylocalFiles.deletef(name);
		deletef(name);
	}else{
		responderClienteRemoto(idG, mensaje::archivoOcupado());
	}
}
void caseWorkerOpenRead(WorkerScope *who,task& t){
	string name  = t.getFileName();
	GlobalId idG = t.getIdGlobal();
	fileStatus state = who->MylocalFiles.status(name);
	
	if (state == noFile){
		responderClienteRemoto(idG,mensaje::archivoNoExiste());
	}else if (state == writing){
		responderClienteRemoto(idG, mensaje::archivoOcupado());
	}else{
		RealFSHandle handle = openr(name);
		LocalFd localFd = who->MyfdManage.registerFd(idG, name, handle);
		who->MylocalFiles.openR (name);
		WorkerId wID = who->MyIdsManage.myId();
		GlobalFd gFd = idsManage::makeGlobalFd(localFd, wID);
		ClientId  c  = idsManage::globalIdToClient(idG);
		WorkerId  w  = idsManage::globalIdToWorker(idG);
		task order   = task::crear_workerOpenSucc(gFd, c);
		enviarWorker(w, order);		
	}
}
void caseWorkerOpenWrite(WorkerScope *who,task& t){
	string name  = t.getFileName();
	GlobalId idG = t.getIdGlobal();
	fileStatus state = who->MylocalFiles.status(name);
	if (state == noFile){
		responderClienteRemoto(idG,mensaje::archivoNoExiste());
	}else if (state != unused){
		responderClienteRemoto(idG, mensaje::archivoOcupado());
	}else{
		RealFSHandle handle = openw(name);
		LocalFd localFd = who->MyfdManage.registerFd(idG, name, handle);
		who->MylocalFiles.openW(name);
		WorkerId wID = who->MyIdsManage.myId();
		GlobalFd gFd = idsManage::makeGlobalFd(localFd, wID);
		ClientId  c  = idsManage::globalIdToClient(idG);
		WorkerId  w  = idsManage::globalIdToWorker(idG);
		task order   = task::crear_workerOpenSucc(gFd, c);
		enviarWorker(w, order);		
	}
}
void caseWorkerOpenSucc(WorkerScope *who,task& t){
	ClientId c = t.getCliente();
	GlobalFd gFd = t.getGlobalFd();
	who->MyopenedFiles.registerOpen(gFd,c);
	responderCliente(c, mensaje::archivoAbierto(gFd), who);
}
void caseWorkerRead(WorkerScope *who,task& t){
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = t.getIdGlobal();
	LocalFd fd = idsManage::globalFdToLocalFd(gFd);
	if (who->MyfdManage.getOwner(fd) != idG){
		responderClienteRemoto(idG,mensaje::permisoDenegado());
	}else{
		RealFSHandle handle = who->MyfdManage.getHandle(fd);
		int sz = t.getSizeTxt();
		string txt;
		if(read(handle,sz,txt)){
			responderClienteRemoto( idG, mensaje::archivoReadSucc(txt));
		}else{
			responderClienteRemoto( idG, mensaje::finDeArchivo());
		}
	}
}
void caseWorkerWrite(WorkerScope *who,task& t){
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = t.getIdGlobal();
	LocalFd fd = idsManage::globalFdToLocalFd(gFd);
	if (who->MyfdManage.getOwner(fd) != idG){
		responderClienteRemoto(idG,mensaje::permisoDenegado());
	}else{
		RealFSHandle handle = who->MyfdManage.getHandle(fd);
		string txt = t.getStrTxt();
		write(handle, txt);
		responderClienteRemoto( idG, mensaje::archivoWriteSucc());
		
	}
}
void caseWorkerSay(WorkerScope *who,task& t){
	ClientId c = t.getCliente();
	mensaje msj = t.getMensaje();
	responderCliente(c, msj, who);
	
}

void caseWorkerClose(WorkerScope *who,task& t){
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = t.getIdGlobal();
	LocalFd fd = idsManage::globalFdToLocalFd(gFd);
	if (who->MyfdManage.getOwner(fd) != idG){
		responderClienteRemoto(idG,mensaje::permisoDenegado());
	}else{
		string file = who->MyfdManage.getNameFile(fd);
		who->MyfdManage.unregisterFd(fd);
		who->MylocalFiles.close(file);
		task order = task::crear_workerCloseSucc(gFd, idG);
		WorkerId  w  = idsManage::globalIdToWorker(idG);
		enviarWorker(w,order);
	}
}
void caseWorkerCloseBye(WorkerScope *who,task& t){
	GlobalFd gFd = t.getGlobalFd();
	LocalFd fd = idsManage::globalFdToLocalFd(gFd);
	string file = who->MyfdManage.getNameFile(fd);
	who->MyfdManage.unregisterFd(fd);
	who->MylocalFiles.close(file);
	who->MyopenedFiles.registerClose(gFd);
}
void caseWorkerCloseSucc(WorkerScope *who,task& t){
	GlobalFd gFd = t.getGlobalFd();
	GlobalId idG = t.getIdGlobal();
	ClientId cID = idsManage::globalIdToClient(idG);
	who->MyopenedFiles.registerClose(gFd);
	responderCliente(cID,mensaje::archivoCerrado(), who);
}
void caseWorkerToken(WorkerScope *who,task& t){
	token tk = t.getToken();
	who->MytokenControl.recvT(tk);
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
			caseUserWrite(who,t);
		break;
		case userRead:
			caseUserRead(who,t);
		break;
		case userClose:
			caseUserClose(who,t);
		break;
		case userBye:
			caseUserBye(who,t);
		break;
		case workerDelete:
			caseWorkerDelete(who,t);
		break;
		case workerOpenRead:
			caseWorkerOpenRead(who,t);
		break;
		case workerOpenWrite:
			caseWorkerOpenWrite(who,t);
		break;
		case workerRead:
			caseWorkerRead(who,t);
		break;
		case workerWrite:
			caseWorkerWrite(who,t);
		break;
		case workerSay:
			caseWorkerSay(who,t);
		break;
		case workerOpenSucc:
			caseWorkerOpenSucc(who,t);
		break;
		case workerClose:
			caseWorkerClose(who,t);
		break;
		case workerCloseBye:
			caseWorkerCloseBye(who,t);
		break;
		case workerCloseSucc:
			caseWorkerCloseSucc(who,t);
		break;
		case workerToken:
			caseWorkerToken(who,t);
		break;
	}
}
