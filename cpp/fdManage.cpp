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
LocalFd fdManage::registerFd(const GlobalId& g,const string& s,const RealFSHandle& h) {
	assert( m.find(counter) == m.end() );
	m[counter]=fd_info(s,g,h);
	return counter++;
}

fdManage::fdManage() : counter(0) {}

