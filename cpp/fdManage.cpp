#include "fdManage.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

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

#ifdef DEBUG_FLAG
string fdManage::say() const {
	stringstream r;
	forall(it,m) r << "(" <<it->fst << "," << it->snd.n << "," << it->snd.g.fst << "," << it->snd.g.snd  << ") ";
	r << endl;
	return r.str();
}
#endif

