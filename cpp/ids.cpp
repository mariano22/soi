#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

typedef int WorkerId;
WorkerId noWorkerId=-1;
typedef int ClientId;
typedef pair<int,int> GlobalId;

typedef int GlobalFd;
typedef int LocalFd;
class idsManage {
	public:
		static GlobalId  makeIdGlobal(WorkerId wId,ClientId cId) { return make_pair(wId,cId); }
		static WorkerId globalIdToWorker(GlobalId gId) { return gId.first; }
		static ClientId globalIdToClient(GlobalId gId) { return gId.second; }

		static WorkerId globalFdToWorker(GlobalFd gFd) { return gFd%10; }
		static LocalFd globalFdToLocalFd(GlobalFd gFd) { return gFd/10; }
		static GlobalFd makeGlobalFd(LocalFd lFd, WorkerId wId) { return lFd*10+wId; }
		
		idsManage(WorkerId myWId) : myWId(myWId) {}
		WorkerId myId() { return myWId; }
		//~ WorkerId nextWorkerId() { return (myWId+1)%workerDirs::wlen(); } inlcuir workerDirs y descomentar linea
	private:
		WorkerId myWId;
};

int main() {
	return 0;
}
