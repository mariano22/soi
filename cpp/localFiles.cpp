#include "localFiles.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

fileStatus localFiles::status(const string& fileName) {
	map<string,status_info>::iterator it = m.find(fileName);
	if (it==m.end()) return noFile ; 
	if ( it->second.lectores ) return reading;
	if ( it->second.escritores ) return writing;
	return unused;
}
void localFiles::deletef (const string& fileName) {
	assert( m.find(fileName) != m.end() );
	assert ( !m[fileName].lectores && !m[fileName].escritores);
	m.erase(fileName);
}
void localFiles::create(const string& fileName) {
	assert( m.find(fileName) == m.end() );
	m[fileName];
}
void localFiles::openR(const string& fileName) {
	map<string,status_info>::iterator it = m.find(fileName);
	assert (it!=m.end()) ; 
	assert( !it->second.escritores ) ;
	it->second.lectores++ ;
}
void localFiles::openW(const string& fileName) {
	map<string,status_info>::iterator it = m.find(fileName);
	assert (it!=m.end()) ; 
	assert( !it->second.lectores ) ;
	it->second.escritores++ ;
}
void localFiles::close(const string& fileName) {
	map<string,status_info>::iterator it = m.find(fileName);
	assert (it!=m.end()) ; 
	assert( it->second.lectores || it->second.escritores ) ;
	if (it->second.lectores) it->second.lectores--;
	else it->second.escritores--;
}

#ifdef DEBUG_FLAG
string localFiles::say() const {
	stringstream r;
	forall(it,m) { 
		r << "(" <<it->fst << "," ;
		if (it->snd.escritores) r << "escribiendo";
		else if (it->snd.lectores) r << it->snd.lectores <<" lectores";
		else r << "libre";
		r<< ") ";
	}
	r << endl;
	return r.str();
}
#endif
