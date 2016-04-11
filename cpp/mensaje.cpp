#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
using namespace std;


typedef int GlobalFd;
typedef int ClientId;
/* estas funciones deberian ir en ids.h */
string globalFdToString(const GlobalFd& gFd) { 
	stringstream ss;
	ss << gFd;
	return ss.str();
}
string clientIdToString(const ClientId& cId) { 
	stringstream ss;
	ss << cId;
	return ss.str();
}

class mensaje {
	public:
		static mensaje mOk() { 
			mensaje r; 
			r.setHeader("OK");
			return r;
		}
		static mensaje mErr() { 
			mensaje r; 
			r.setHeader("ERROR");
			return r;
		}
		static mensaje archivoNoExiste() { 
			mensaje r = mErr();
			r.addArg("notExist");
			return r;
		}
		static mensaje archivoOcupado() { 
			mensaje r = mErr();
			r.addArg("Ocuppied");
			return r;
		}
		static mensaje archivoExistente() { 
			mensaje r = mErr();
			r.addArg("Exist");
			return r;
		}
		static mensaje permisoDenegado() { 
			mensaje r = mErr();
			r.addArg("AccessDenied");
			return r;
		}
		static mensaje finDeArchivo() { 
			mensaje r = mErr();
			r.addArg("EOF");
			return r;
		}

		static mensaje archivoCreado() { 
			mensaje r = mOk();
			r.addArg("Create");
			return r;
		}
		static mensaje archivoBorrado() { 
			mensaje r = mOk();
			r.addArg("Deleted");
			return r;
		}
		static mensaje archivoCerrado() { 
			mensaje r = mOk();
			r.addArg("Close");
			return r;
		}
		static mensaje archivoWriteSucc() { 
			mensaje r = mOk();
			r.addArg("Writed");
			return r;
		}
		static mensaje archivoReadSucc(const string& Txt) { 
			mensaje r = mOk();
			r.addArg(Txt);
			return r;
		}
		static mensaje archivoAbierto(const GlobalFd& gFd) { 
			mensaje r = mOk();
			r.addArg(globalFdToString(gFd));
			return r;
		}
		static mensaje coneccionEstablecida(const ClientId& cId) { 
			mensaje r = mOk();
			r.addArg(clientIdToString(cId));
			return r;
		}
		
		string say() {
			string res = header + (int(args.size()) ? " " : "\n");
			for(int i=0 ; i < int(args.size()) ; i++) {
				res = res + args[i] + ( i==int(args.size())-1 ? "\n" : " " );
			}
			return res;
		}
	private:
		void setHeader(const string& h) { header = h; }
		void addArg(const string& arg) { args.push_back(arg); }
		string header;
		vector<string> args;
		
};



int main() {
	return 0;
}
