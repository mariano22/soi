#include "fdManage.h"

RealFSHandle fdManage::getHandle(const LocalFd& fd) {
	map<LocalFd,fd_info>::iterator it = m.find(fd);
	assert( it != m.end() );
	return it->second.h;
}
GlobalId fdManage::getOwner(const LocalFd& fd) {
	map<LocalFd,fd_info>::iterator it = m.find(fd);
	assert( it != m.end() );
	return it->second.g;
}
string fdManage::getNameFile(const LocalFd& fd) {
	map<LocalFd,fd_info>::iterator it = m.find(fd);
	assert( it != m.end() );
	return it->second.n;
}
void fdManage::unregisterFd(const LocalFd& fd) {
	assert( m.find(fd) != m.end() );
	m.erase(fd);
}
void fdManage::registerFd(const LocalFd& fd,const GlobalId& g,const string& s,const RealFSHandle& h) {
	assert( m.find(fd) == m.end() );
	m[fd]=fd_info(s,g,h);
}
/*
struct fd_info {
	string n;
	GlobalId g;
	RealFSHandle h;
	fd_info() {}
	fd_info(const string& n,const GlobalId& g,const RealFSHandle& h) : n(n), g(g), h(h) {}
};
map< LocalFd, fd_info > m;
*/
