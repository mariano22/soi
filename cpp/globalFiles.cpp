#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;
//~ % Funciones:
//~ % archivosActuales () -> [String] (Lista los archivos que actualmente es la base de datos global del worker)
//~ % getOwner : String -> noOwner | WorkerId (Dado un archivo, si existe devuelve su dueño)
//~ % baja : String -> ok (Da de baja un archivo en la tabla)
//~ % alta : String, WorkerId -> ok (Da de alta un archivo en la tabla, asignandole un dueño)

typedef int WorkerId;
WorkerId noWorkerId=-1;

class globalFiles {
	public:
		vector<string> archivosActuales() {
			vector<string> result;
			for(map<string,WorkerId>::iterator it = m.begin() ; it != m.end() ; ++it) 
				result.push_back(it->first);
			return result;
		}
		WorkerId getOwner(const string& fileName) {
			map<string,WorkerId>::iterator it = m.find(fileName);
			return it==m.end() ? noWorkerId : it->second;
		}
		void baja(const string& fileName) {
			assert( m.find(fileName) != m.end() );
			m.erase(fileName);
		}
		void alta(const string& fileName, WorkerId id) {
			assert( m.find(fileName) == m.end() );
			m[fileName] = id;
		}
	private:
		map< string,WorkerId > m;
};

int main() {
	return 0;
}
