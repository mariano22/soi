#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;
//~ % Funciones:
//~ % getHandle : LocalFd -> Handle (Dado un Fd devuelve el Handle del sistema de archivos real)
//~ % getOwner  : LocalFd -> GlobalId (Dado un Fd devuelve el usuario que lo posee)
//~ % getNameFile  : LocalFd -> String (Dado un Fd devuelve el nombre del archivo que es)
//~ % unregisterFd : LocalFd -> ok (Borra un Fd de la tabla)
//~ % registerFd : Globalid,NameFile,Handle -> LocalFd (Registra un nuevo Fd la tabla y devuelve le Fd asignado)

typedef int LocalFd;
typedef pair<int,int> GlobalId;
typedef int RealFSHandle;


class fdManage {
	public:
		RealFSHandle getHandle(const LocalFd& fd) {
			map<LocalFd,fd_info>::iterator it = m.find(fd);
			assert( it != m.end() );
			return it->second.h;
		}
		GlobalId getOwner(const LocalFd& fd) {
			map<LocalFd,fd_info>::iterator it = m.find(fd);
			assert( it != m.end() );
			return it->second.g;
		}
		string getNameFile(const LocalFd& fd) {
			map<LocalFd,fd_info>::iterator it = m.find(fd);
			assert( it != m.end() );
			return it->second.n;
		}
		void unregisterFd(const LocalFd& fd) {
			assert( m.find(fd) != m.end() );
			m.erase(fd);
		}
		void registerFd(const LocalFd& fd,const GlobalId& g,const string& s,const RealFSHandle& h) {
			assert( m.find(fd) == m.end() );
			m[fd]=fd_info(s,g,h);
		}
	private:
		struct fd_info {
			string n;
			GlobalId g;
			RealFSHandle h;
			fd_info() {}
			fd_info(const string& n,const GlobalId& g,const RealFSHandle& h) : n(n), g(g), h(h) {}
		};
		map< LocalFd, fd_info > m;
};

int main() {
	return 0;
}
