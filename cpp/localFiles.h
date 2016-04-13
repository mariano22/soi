#ifndef LOCALFILES_H
#define LOCALFILES_H

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
		fileStatus status(const string& fileName);
		void deletef (const string& fileName);
		void create(const string& fileName);
		void openR(const string& fileName);
		void openW(const string& fileName);
		void close(const string& fileName);
	private://ver como importar/exportar
		struct status_info {
			int lectores,escritores;
			status_info(int lectores=0,int escritores=0) : lectores(lectores), escritores(escritores) {}
		};
		map< string,status_info > m;
};
#endif
