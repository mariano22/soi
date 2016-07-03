#include <bits/stdc++.h>
#include "globalFiles.h"
#include "fdManage.h"
#include "localFiles.h"
#include "openedFiles.h"
#include "tokenControl.h"
#include "workerDirs.h"
#include "task.h"
#include "syncQueues.h"
using namespace std;

class WorkerScope {
	public:
		globalFiles MyglobalFiles;
		fdManage MyfdManage;
		localFiles MylocalFiles;
		openedFiles MyopenedFiles;
		tokenControl MytokenControl;
		tokenQueues MytokenQueues;
		syncQueues MysyncQueues;
};



