#include "tokenQueues.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second


void tokenQueues::newCreate(const string& fileName, ClientId cid) {
	assert( c.find(fileName) == c.end() );
	c[fileName]=cid;
}
void tokenQueues::newDelete(const string& fileName) {
	assert( d.find(fileName) == d.end() );
	d.insert(fileName);
}
vector< pair<string,ClientId> > tokenQueues::getCreates() {
	// DEBUG
	//~ cout << "DEBUG 1: " << c.size()  << endl;
	//~ if (c.size()) cout << "HAY ALGO INTERESANTE!!!!!!!!!!!" << endl;
	vector< pair<string,ClientId> > result;
	for(map<string,ClientId>::iterator it = c.begin() ; it != c.end() ; ++it) 
		result.push_back(*it);
	c.clear();
	return result;
}
vector< string > tokenQueues::getDeletes() {
	vector< string > result;
	for(set<string>::iterator it = d.begin() ; it != d.end() ; ++it) 
		result.push_back(*it);
	d.clear();
	return result;
}
bool tokenQueues::isInCreate(string fileName) {
	return c.find(fileName) != c.end();
}

#ifdef DEBUG_FLAG
string tokenQueues::say() {
	stringstream r;
	r << "Deletes pendientes: " ;
	forall(it,d) r << *it <<"," ;
	r << endl << "Altas pendientes: ";
	forall(it,c) r << "(" << it->fst << " , " << it->snd << ") ; " ;
	r << endl;
	return r.str();
}
#endif
