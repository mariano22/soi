#ifndef WORKERSCOPE_H
#define WORKERSCOPE_H

#include <bits/stdc++.h>
#include "globalFiles.h"
#include "fdManage.h"
#include "localFiles.h"
#include "openedFiles.h"
#include "tokenControl.h"
#include "tokenQueues.h"
#include "task.h"
#include "syncQueues.h"
#include "localConections.h"
using namespace std;

class WorkerScope {
	public:
		globalFiles MyglobalFiles;
		fdManage MyfdManage;
		localFiles MylocalFiles;
		openedFiles MyopenedFiles;
		tokenControl MytokenControl;
		tokenQueues MytokenQueues;
		syncQueues<task> *MysyncQueues;
		localConections *MylocalConections;
		idsManage MyIdsManage;
		WorkerScope(WorkerId myWId) : MyIdsManage(myWId) {}
};

#endif
