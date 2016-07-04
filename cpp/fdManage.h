#ifndef FDMANAGE_H
#define FDMANAGE_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <string> 
#include "realFs.h"
#include "ids.h"
using namespace std;

class fdManage {
	public:
		RealFSHandle getHandle(const LocalFd& fd);
		GlobalId getOwner(const LocalFd& fd);
		string getNameFile(const LocalFd& fd);
		void unregisterFd(const LocalFd& fd);
		LocalFd registerFd(const GlobalId& g,const string& s,const RealFSHandle& h);
		fdManage();
		#ifdef DEBUG_FLAG
		string say() const;
		#endif
	private:/*ver como hacer en el .cpp*/
		struct fd_info {
			string n;
			GlobalId g;
			RealFSHandle h;
			fd_info() {}
			fd_info(const string& n,const GlobalId& g,const RealFSHandle& h) : n(n), g(g), h(h) {}
		};
		map< LocalFd, fd_info > m;
		int counter;
};


#endif
