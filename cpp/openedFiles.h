#ifndef OPENEDFILES_H
#define OPENEDFILES_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include "ids.h"

class openedFiles {
	public:
		vector<GlobalFd> globalFdList(const ClientId& cId);
		void registerClose(const GlobalFd& gFd);
		void registerOpen(const GlobalFd& gFd, const ClientId& cId);
		#ifdef DEBUG_FLAG
		string say() const;
		#endif
	private:
		map< GlobalFd,ClientId > m;
};

#endif
