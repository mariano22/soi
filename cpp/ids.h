#ifndef IDS_H
#define IDS_H

#include <utility> 
#include <string> 
extern int workerCant; // de workerMain
using namespace std;

typedef int WorkerId;
const WorkerId noWorkerId=-1;
typedef int ClientId;
const ClientId noClientId=-1;
typedef pair<int,int> GlobalId;
typedef int ClientId;
typedef int GlobalFd;
typedef int LocalFd;

string   globalFdToString(const GlobalFd& gFd);
string   clientIdToString(const ClientId& cId);
GlobalFd stringToGlobalFd(const string& sgFd);
class idsManage {
	public:
		static GlobalId makeIdGlobal(WorkerId wId,ClientId cId);
		static WorkerId globalIdToWorker(GlobalId gId);
		static ClientId globalIdToClient(GlobalId gId);

		static WorkerId globalFdToWorker(GlobalFd gFd);
		static LocalFd  globalFdToLocalFd(GlobalFd gFd);
		static GlobalFd makeGlobalFd(LocalFd lFd, WorkerId wId);
		
		idsManage(WorkerId myWId) : myWId(myWId) {}
		WorkerId myId();
		WorkerId nextWorkerId();
	private:
		WorkerId myWId;
};
#endif
