#include "ids.h"

static GlobalId idsManage::makeIdGlobal(WorkerId wId,ClientId cId) { return make_pair(wId,cId); }
static WorkerId idsManage::globalIdToWorker(GlobalId gId) { return gId.first; }
static ClientId idsManage::globalIdToClient(GlobalId gId) { return gId.second; }

static WorkerId idsManage::globalFdToWorker(GlobalFd gFd) { return gFd%10; }
static LocalFd  idsManage::globalFdToLocalFd(GlobalFd gFd) { return gFd/10; }
static GlobalFd idsManage::makeGlobalFd(LocalFd lFd, WorkerId wId) { return lFd*10+wId; }

WorkerId idsManage::myId() { return myWId; }
//~ WorkerId nextWorkerId() { return (myWId+1)%workerDirs::wlen(); } inlcuir workerDirs y descomentar linea

WorkerId idsManage::myWId;
