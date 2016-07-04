#include "globalFiles.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

vector<string> globalFiles::archivosActuales() {
	vector<string> result;
	for(map<string,WorkerId>::iterator it = m.begin() ; it != m.end() ; ++it) 
		result.push_back(it->first);
	return result;
}
WorkerId globalFiles::getOwner(const string& fileName) {
	map<string,WorkerId>::iterator it = m.find(fileName);
	return it==m.end() ? noWorkerId : it->second;
}
void globalFiles::baja(const string& fileName) {
	assert( m.find(fileName) != m.end() );
	m.erase(fileName);
}
void globalFiles::alta(const string& fileName, WorkerId id) {
	assert( m.find(fileName) == m.end() );
	m[fileName] = id;
}

#ifdef DEBUG_FLAG
string globalFiles::say() const {
	stringstream r;
	forall(it,m) r << "(" <<it->fst << "," << it->snd << ") ";
	r << endl;
	return r.str();
}
#endif

