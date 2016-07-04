#ifndef GLOBALFILES_H
#define GLOBALFILES_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include "ids.h"
using namespace std;


class globalFiles {
	public:
		vector<string> archivosActuales();
		WorkerId getOwner(const string& fileName);
		void baja(const string& fileName);
		void alta(const string& fileName, WorkerId id);
		#ifdef DEBUG_FLAG
		string say() const;
		#endif
	private:
		map< string,WorkerId > m;
};


#endif
