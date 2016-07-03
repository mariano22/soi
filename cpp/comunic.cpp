#include "comunic.h"
#include "mainWorker.h"

void enviarWorker(const WorkerId& dest,const task& t) {
	workerQueues[dest].push(t);
}
void responderCliente(const ClientId& dest,const mensaje& txt, WorkerScope* ws) {
	ws->MylocalConections.find(dest)->push(txt);
}
