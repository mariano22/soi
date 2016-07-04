#include "dispatcher.h"
#define DISPATCHER_EXTERN_PORT 8085
using namespace std;

void *clientSlave(void *arg) {
	
	cout << "asdasd " << endl;
	
	int conn_s = ((pair<int,int> *)arg)->first;
	int myId = ((pair<int,int> *)arg)->second;
	delete ((pair<int,int> *)arg);
	int wId = rand() % workerCant;
	syncQueues<mensaje> *myInbox = new syncQueues<mensaje>;
	workerConections[wId].newC(myId,myInbox);
	
	cout << "Nuevo Cliente " << myId << " asosciado a " << wId << endl;
	
	bool exit_flag;
	do {
	string data;
    exit_flag = getLineSocket(conn_s,data);
    vector<string> pData = parser(data);
    pair<task,bool> result = task::fromUserData(pData,myId);
    
    if (result.second==false) write(conn_s,"Invalid Command\n",16);
    else {
		
		workerQueues[wId].push(result.first);
		if (result.first.getTaskName() == userBye) break;
		else {
			mensaje respuesta; 
			myInbox->recv(respuesta,-1);
			string Sr = respuesta.say();
			write(conn_s,Sr.c_str(),Sr.size());
		}
	}
    
  } while (exit_flag);
  
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

  for (int cId=0;;cId++) {
    assert( (conn_s = accept(list_s, NULL, NULL) ) >= 0 );
    pair<int,int> *arg = new pair<int,int>(conn_s,cId);
    pthread_t tmp_thread; pthread_create(&tmp_thread,0,clientSlave,arg);
  }
}
