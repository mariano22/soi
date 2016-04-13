#ifndef IDS_H
#define IDS_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

//ver de donde se importa
typedef int WorkerId;
WorkerId noWorkerId=-1;
typedef int ClientId;
typedef pair<int,int> GlobalId;

typedef int GlobalFd;
typedef int LocalFd;
class idsManage {
	public:
		static GlobalId  makeIdGlobal(WorkerId wId,ClientId cId);
		static WorkerId globalIdToWorker(GlobalId gId);
		static ClientId globalIdToClient(GlobalId gId);

		static WorkerId globalFdToWorker(GlobalFd gFd);
		static LocalFd globalFdToLocalFd(GlobalFd gFd);
		static GlobalFd makeGlobalFd(LocalFd lFd, WorkerId wId);
		
		idsManage(WorkerId myWId) : myWId(myWId) {}
		WorkerId myId();
		//~ WorkerId nextWorkerId() { return (myWId+1)%workerDirs::wlen(); } inlcuir workerDirs y descomentar linea
	private:
		WorkerId myWId;
};
#endif
