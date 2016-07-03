#ifndef COMUNIC_H
#define COMUNIC_H

#include <bits/stdc++.h>
#include "ids.h"
#include "task.h"
#include "mensaje.h"
#include "workerScope.h"
using namespace std;

void enviarWorker(const WorkerId& dest,const task& t);
void responderCliente(const ClientId& dest,const mensaje& txt, WorkerScope* ws);

#endif

