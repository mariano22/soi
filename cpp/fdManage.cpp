#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;
//~ % Funciones:
//~ % getOwner  : LocalFd -> GlobalId (Dado un Fd devuelve el usuario que lo posee)
//~ % getNameFile  : LocalFd -> String (Dado un Fd devuelve el nombre del archivo que es)
//~ % unregisterFd : LocalFd -> ok (Borra un Fd de la tabla)
//~ % registerFd : Globalid,NameFile -> LocalFd (Registra un nuevo Fd la tabla y devuelve le Fd asignado)

typedef int LocalFd;
typedef pair<int,int> GlobalId;


class fdManage {
	public:
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
		void registerFd(const LocalFd& fd,const GlobalId& g,const string& s) {
			assert( m.find(fd) == m.end() );
			m[fd]=fd_info(s,g);
		}
	private:
		struct fd_info {
			string n;
			GlobalId g;
			fd_info() {}
			fd_info(const string& n,const GlobalId& g) : n(n), g(g) {}
		};
		map< LocalFd, fd_info > m;
};

int main() {
	return 0;
}
