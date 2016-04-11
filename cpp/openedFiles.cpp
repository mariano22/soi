#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

//~ % Funciones
//~ % registerOpen : GlobalFd, ClientId -> ok (Registra que el GlobalFd fue tomado por el ClientId)
//~ % registerClose : GlobalFd -> ok (Elimina a GlobalFd de la tabla, para cuando el cliente cierra el archivo)
//~ % globalFdList : ClientId -> [GlobalFd] (Lista los GlobalFd que actualmente tiene en posecion el cliente, muy util a la hora de hacer un bye)


typedef pair<int,int> GlobalFd;
typedef int ClientId;

class openedFiles {
	public:
		vector<GlobalFd> globalFdList(const ClientId& cId) {
			vector<GlobalFd> result;
			for(map<GlobalFd,ClientId>::iterator it = m.begin() ; it != m.end() ; ++it) 
				if ( it->second == cId )
					result.push_back(it->first);
			return result;
		}
		void registerClose(const GlobalFd& gFd) {
			assert( m.find(gFd) != m.end() );
			m.erase(gFd);
		}
		void registerOpen(const GlobalFd& gFd, const ClientId& cId) {
			assert( m.find(gFd) == m.end() );
			m[gFd] = cId;
		}
	private:
		map< GlobalFd,ClientId > m;
};

int main() {
	return 0;
}
