#include "openedFiles.h"

vector<GlobalFd> openedFiles::globalFdList(const ClientId& cId) {
	vector<GlobalFd> result;
	for(map<GlobalFd,ClientId>::iterator it = m.begin() ; it != m.end() ; ++it) 
		if ( it->second == cId )
			result.push_back(it->first);
	return result;
}

void openedFiles::registerClose(const GlobalFd& gFd) {
	assert( m.find(gFd) != m.end() );
	m.erase(gFd);
}

void openedFiles::registerOpen(const GlobalFd& gFd, const ClientId& cId) {
	assert( m.find(gFd) == m.end() );
	m[gFd] = cId;
}




map< GlobalFd,ClientId > m;//QUE ONDA? ¿=?

