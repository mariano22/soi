#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#include "ids.h"

class mensaje {
	public:
		static mensaje mOk();
		static mensaje mErr();
		static mensaje archivoNoExiste();
		static mensaje archivoOcupado();
		static mensaje archivoExistente();
		static mensaje permisoDenegado();
		static mensaje finDeArchivo();

		static mensaje archivoCreado();
		static mensaje archivoBorrado();
		static mensaje archivoCerrado();
		static mensaje archivoWriteSucc();
		static mensaje archivoReadSucc(const string& Txt);
		static mensaje archivoAbierto(const GlobalFd& gFd);
		static mensaje coneccionEstablecida(const ClientId& cId);

		void addArg(const string& arg);		
		string say();
	private:
		void setHeader(const string& h);

		string header;
		vector<string> args;
		
};

#endif
