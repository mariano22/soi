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
		#ifdef DEBUG_FLAG
		string say() {
			stringstream r;
			r << "WorkerScope de " << MyIdsManage.myId() << ":\n";
			r << "globalFiles: " << MyglobalFiles.say() << endl;
			r << "localFiles: " << MylocalFiles.say() << endl;
			r << "openedFiles: " << MyopenedFiles.say() << endl;
			r << "fdManage: " << MyfdManage.say() << endl;
			r << "tokenControl: " << MytokenControl.say() << endl;
			r << "tokenQueues: " << MytokenQueues.say() << endl;
			return r.str();
		}
		#endif
};

#endif
