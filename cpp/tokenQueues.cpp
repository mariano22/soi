#include "tokenQueues.h"


void tokenQueues::newCreate(const string& fileName, ClientId cid) {
	assert( c.find(fileName) == c.end() );
	c[fileName]=cid;
}
void tokenQueues::newDelete(const string& fileName) {
	assert( d.find(fileName) == d.end() );
	d.insert(fileName);
}
vector< pair<string,ClientId> > tokenQueues::getCreates() {
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
	c.clear();
	return result;
}


