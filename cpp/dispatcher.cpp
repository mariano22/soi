#include "dispatcher.h"
#define DISPATCHER_EXTERN_PORT 8080
using namespace std;

void *clientSlave(void *arg) {
	int conn_s = ((pair<int,int> *)arg)->first;
	int myId = ((pair<int,int> *)arg)->second;
	delete ((pair<int,int> *)arg);
	int wId = rand() % workerCant;
	syncQueues<mensaje> *myInbox = new syncQueues<mensaje>;
	workerConections[wId].newC(myId,myInbox);
	
	/*#ifdef DEBUG_FLAG
	cout << "DISPATCHER: Nuevo Cliente #" << myId << " asosciado a " << wId << endl;
	#endif*/
	
	bool exit_flag;
	do {
		string data;
		exit_flag = getLineSocket(conn_s,data);
		vector<string> pData = parser(data);
		pair<task,bool> result = task::fromUserData(pData,myId);
		
		#ifdef DEBUG_FLAG
		cout << "DISPATCHER: recibiendo mensaje de " << myId << ": " << data << endl;
		#endif
		
		if (result.second==false) write(conn_s,"Invalid Command\n",16);
		else {	
			workerQueues[wId].push(result.first);
			
			/*#ifdef DEBUG_FLAG
			cout << "DISPATCHER: task enviado, esperando respuesta " << myId << endl;
			#endif*/
			
			mensaje respuesta; 
			myInbox->recv(respuesta,-1);
			string Sr = respuesta.say();
			write(conn_s,Sr.c_str(),Sr.size());
			exit_flag = result.first.getTaskName()==userBye;
		}
		
  } while (!exit_flag);
  
	/*#ifdef DEBUG_FLAG
	cout << "DISPATCHER: cliente terminando " << myId << endl;
	#endif*/
  
  close(conn_s);
  workerConections[wId].delC(myId);
  delete myInbox;
  return NULL;
}


void launchDispatcher()
{  
  int list_s,conn_s=-1;
  struct sockaddr_in servaddr;
  assert ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) >= 0 );
  
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(DISPATCHER_EXTERN_PORT);

  assert( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) >= 0 );
  assert( listen(list_s, 10) >= 0 );
  
  /*#ifdef DEBUG_FLAG
  cout << "DISPATCHER: listo para recibir conexiones en " << DISPATCHER_EXTERN_PORT << endl;
  #endif*/

  for (int cId=0;;cId++) {
    assert( (conn_s = accept(list_s, NULL, NULL) ) >= 0 );
    pair<int,int> *arg = new pair<int,int>(conn_s,cId);
    pthread_t tmp_thread; pthread_create(&tmp_thread,0,clientSlave,arg);
  }
}
