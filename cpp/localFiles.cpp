#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;
//~ % Funciones
//~ % status : String -> noFile | reading | writing | unused (Dado un archivo, si esta localmente devuelve el estado)
//~ % deletef : String -> ok (Dado un archivo lo borra de la tabla de los locales)
//~ % create : String -> ok (Dado un archivo lo crea en la tabla de los locales, inicialmente unused)
//~ % openR : String -> ok (Suma un lector al archivo)
//~ % openW : String -> ok (Suma un escritor al archivo, el llamante se asegura que nunca halla mas de uno)
//~ % close : String -> ok (Resta un usuario al archivo, detectando si esta siendo leido o escrito)
enum fileStatus { noFile, reading, writing, unused };
class localFiles {
	public:
		fileStatus status(const string& fileName) {
			map<string,status_info>::iterator it = m.find(fileName);
			if (it==m.end()) return noFile ; 
			if ( it->second.lectores ) return reading;
			if ( it->second.escritores ) return writing;
			return unused;
		}
		void deletef (const string& fileName) {
			assert( m.find(fileName) != m.end() );
			assert ( !m[fileName].lectores && !m[fileName].escritores);
			m.erase(fileName);
		}
		void create(const string& fileName) {
			assert( m.find(fileName) == m.end() );
			m[fileName];
		}
		void openR(const string& fileName) {
			map<string,status_info>::iterator it = m.find(fileName);
			assert (it!=m.end()) ; 
			assert( !it->second.escritores ) ;
			it->second.lectores++ ;
		}
		void openW(const string& fileName) {
			map<string,status_info>::iterator it = m.find(fileName);
			assert (it!=m.end()) ; 
			assert( !it->second.lectores ) ;
			it->second.escritores++ ;
		}
		void close(const string& fileName) {
			map<string,status_info>::iterator it = m.find(fileName);
			assert (it!=m.end()) ; 
			assert( it->second.lectores || it->second.escritores ) ;
			if (it->second.lectores) it->second.lectores--;
			else it->second.escritores--;
		}
	private:
		struct status_info {
			int lectores,escritores;
			status_info(int lectores=0,int escritores=0) : lectores(lectores), escritores(escritores) {}
		};
		map< string,status_info > m;
};

int main() {
	return 0;
}
