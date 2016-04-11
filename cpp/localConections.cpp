#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;
//~ % Funciones:
//~ % newC : Clientid , WorkerId -> ok (registra que el cliente esta atendida su comunicacion por medio del esclavo cuyo pid es Pid)
//~ % find  : Clientid      -> WorkerId  (devuelve el pid del proceso que atiende la comunicacion de un cliente)
//~ % delC :  Clientid      -> ok  (borra un cliente, porque se desconecto)

typedef int WorkerId;
typedef int ClientId;
const ClientId noClientId=-1; // esta definicion debiera estar en ids.h


class localConections {
	public:
		ClientId getOfindwner(const WorkerId& wId) {
			map<ClientId,WorkerId>::iterator it = m.find(wId);
			return it==m.end() ? noClientId : it->second;
		}
		void baja(const WorkerId& wId) {
			assert( m.find(wId) != m.end() );
			m.erase(wId);
		}
		void alta(const WorkerId& wId, ClientId id) {
			assert( m.find(wId) == m.end() );
			m[wId] = id;
		}
	private:
		map< ClientId,WorkerId > m;
};

int main() {
	return 0;
}
