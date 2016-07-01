#include <vector>
#include <map>
#include <cassert>
#include <algorithm>
#include <utility>
#include <sstream>
#include <string>
#include "ids.h"
using namespace std;

string globalFdToString(const GlobalFd& gFd) { 
	stringstream ss;
	ss << gFd;
	return ss.str();
}
string clientIdToString(const ClientId& cId) { 
	stringstream ss;
	ss << cId;
	return ss.str();
}

GlobalFd stringToGlobalFd(const string& sgFd) {
	stringstream ss(sgFd);
	GlobalFd res;
	ss >> res;
	return res;
} 

GlobalId idsManage::makeIdGlobal(WorkerId wId,ClientId cId) { return make_pair(wId,cId); }
WorkerId idsManage::globalIdToWorker(GlobalId gId) { return gId.first; }
ClientId idsManage::globalIdToClient(GlobalId gId) { return gId.second; }

WorkerId idsManage::globalFdToWorker(GlobalFd gFd) { return gFd%10; }
LocalFd  idsManage::globalFdToLocalFd(GlobalFd gFd) { return gFd/10; }
GlobalFd idsManage::makeGlobalFd(LocalFd lFd, WorkerId wId) { return lFd*10+wId; }

WorkerId idsManage::myId() { return myWId; }
//~ WorkerId nextWorkerId() { return (myWId+1)%workerDirs::wlen(); } inlcuir workerDirs y descomentar linea

