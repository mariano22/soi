#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cassert>
#define forall(it,v) for(typeof(v.begin()) it=v.begin();it!=v.end();++it)
using namespace std;

typedef int WorkerId;

class workerDirs {
	public:	 
		void init(const string& nameFile) {
			ifstream file (nameFile.c_str());
			assert(file.is_open());
			wdir e; int n; file >> n;
			for(int i=0 ; i<n ; i++) { 
				file >> e.ip >> e.puertoExterno >> e.puertoInterno; 
				wdirs.push_back( e );
			}
			file.close();
		}
		int wlen() { return int(wdirs.size()); }
		string ip(const WorkerId& WId) { 
			assert( WId>=0 && WId < int(wdirs.size()) );
			return wdirs[WId].ip;
		}
		int externPort(const WorkerId& WId) { 
			assert( WId>=0 && WId < int(wdirs.size()) );
			return wdirs[WId].puertoExterno;
		}
		int internPort(const WorkerId& WId) { 
			assert( WId>=0 && WId < int(wdirs.size()) );
			return wdirs[WId].puertoInterno;
		}
	private:
		struct wdir {
			string ip;
			int puertoExterno,puertoInterno;
		};
		vector< wdir > wdirs;
};

int main() {
	//~ workerDirs x;
	//~ x.init("workerlist");
	//~ for(int i=0 ; i < x.wlen() ; i++) {
		//~ cout << x.ip(i) <<" " << x.externPort(i) << " "<< x.internPort(i) << " " << endl;
	//~ }
	return 0;
}
