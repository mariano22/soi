#include "localConections.h"
using namespace std;

ClientId localConections::getOfindwner(const WorkerId& wId) {
	map<ClientId,WorkerId>::iterator it = m.find(wId);
	return it==m.end() ? noClientId : it->second;
}
void localConections::baja(const WorkerId& wId) {
	assert( m.find(wId) != m.end() );
	m.erase(wId);
}
void localConections::alta(const WorkerId& wId, ClientId id) {
	assert( m.find(wId) == m.end() );
	m[wId] = id;
}

//		map< ClientId,WorkerId > m;

