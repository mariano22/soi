#include "comunic.h"
#include "mainWorker.h"

void enviarWorker(const WorkerId& dest,const task& t) {
	#ifdef DEBUG_FLAG
	cout << "COMUNIC: respuesta al worker " << dest << " con el task:\n" << t.say() << endl << endl;
	#endif
	workerQueues[dest].push(t);
}
void responderCliente(const ClientId& dest,const mensaje& txt, WorkerScope* ws) {
	#ifdef DEBUG_FLAG
	cout << "COMUNIC: respuesta de worker " << ws->MyIdsManage.myId() << " al cliente " << dest << " contenido: " << txt.say() << endl ;
	#endif
	ws->MylocalConections->find(dest)->push(txt);
}
