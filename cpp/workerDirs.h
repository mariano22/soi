#ifndef WORKERDIRS_H
#define WORKERDIRS_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

typedef int WorkerId;

class workerDirs {
	public:	 
		void init(const string& nameFile);
		int wlen();
		string ip(const WorkerId& WId);
		int externPort(const WorkerId& WId);
		int internPort(const WorkerId& WId);
	private:
		struct wdir {
			string ip;
			int puertoExterno,puertoInterno;
		};
		vector< wdir > wdirs;
};



#endif
