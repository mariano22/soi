#include "workerDirs.h"

using namespace std;

typedef int WorkerId;


void workerDirs::init(const string& nameFile) {
	ifstream file (nameFile.c_str());
	assert(file.is_open());
	wdir e; int n; file >> n;
	for(int i=0 ; i<n ; i++) { 
		file >> e.ip >> e.puertoExterno >> e.puertoInterno; 
		wdirs.push_back( e );
	}
	file.close();
}

int workerDirs::wlen() { return int(wdirs.size()); }

string workerDirs::ip(const WorkerId& WId) { 
	assert( WId>=0 && WId < int(wdirs.size()) );
	return wdirs[WId].ip;
}



int workerDirs::externPort(const WorkerId& WId) { 
	assert( WId>=0 && WId < int(wdirs.size()) );
	return wdirs[WId].puertoExterno;
}

int workerDirs::internPort(const WorkerId& WId) { 
	assert( WId>=0 && WId < int(wdirs.size()) );
	return wdirs[WId].puertoInterno;
}
