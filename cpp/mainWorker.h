#ifndef MAINWORKER_H
#define MAINWORKER_H

#include "syncQueues.h"
#include "task.h"
#include "localConections.h"
extern int workerCant;
extern syncQueues<task> *workerQueues;
extern localConections *workerConections;
void* mainWorker(void *pid);
#endif
