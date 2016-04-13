#ifndef GLOBALFILES_H
#define GLOBALFILES_H

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

//ver de donde importar
typedef int WorkerId;
WorkerId noWorkerId=-1;

class globalFiles {
	public:
		vector<string> archivosActuales();
		WorkerId getOwner(const string& fileName);
		void baja(const string& fileName);
		void alta(const string& fileName, WorkerId id);
	private:/*ver como hacer en el .cpp*/
		map< string,WorkerId > m;
};


#endif
