#include "localFiles.h"

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

	/*	struct status_info {
			int lectores,escritores;
			status_info(int lectores=0,int escritores=0) : lectores(lectores), escritores(escritores) {}
		};
		map< string,status_info > m;
*/