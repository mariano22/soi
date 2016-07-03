#include "localConections.h"
using namespace std;

syncQueues<mensaje>* localConections::find(const ClientId& cId) {
	map<ClientId,syncQueues<mensaje>*>::iterator it = m.find(cId);
	return it==m.end() ? NULL : it->second;
}
void localConections::delC(const ClientId& cId) {
	assert( m.find(cId) != m.end() );
	m.erase(cId);
}
void localConections::newC(const ClientId& cId, syncQueues<mensaje>* qp) {
	assert( m.find(cId) == m.end() );
	m[cId] = qp;
}



