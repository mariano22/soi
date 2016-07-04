#include "openedFiles.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

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

#ifdef DEBUG_FLAG
string openedFiles::say() const {
	stringstream r;
	forall(it,m) r << "(" <<it->fst << "," << it->snd << ") ";
	r << endl;
	return r.str();
}
#endif

