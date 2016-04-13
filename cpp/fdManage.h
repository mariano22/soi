#ifndef FDMANAGE_H
#define FDMANAGE_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

//ver de donde importar
typedef int LocalFd;
typedef pair<int,int> GlobalId;
typedef int RealFSHandle

//~ % Funciones:
//~ % getHandle : LocalFd -> Handle (Dado un Fd devuelve el Handle del sistema de archivos real)
//~ % getOwner  : LocalFd -> GlobalId (Dado un Fd devuelve el usuario que lo posee)
//~ % getNameFile  : LocalFd -> String (Dado un Fd devuelve el nombre del archivo que es)
//~ % unregisterFd : LocalFd -> ok (Borra un Fd de la tabla)
//~ % registerFd : Globalid,NameFile,Handle -> LocalFd (Registra un nuevo Fd la tabla y devuelve le Fd asignado)

class fdManage {
	public:
		RealFSHandle getHandle(const LocalFd& fd);
		GlobalId getOwner(const LocalFd& fd);
		string getNameFile(const LocalFd& fd);
		void unregisterFd(const LocalFd& fd);
		void registerFd(const LocalFd& fd,const GlobalId& g,const string& s,const RealFSHandle& h);
	private:/*ver como hacer en el .cpp*/
		struct fd_info {
			string n;
			GlobalId g;
			RealFSHandle h;
			fd_info() {}
			fd_info(const string& n,const GlobalId& g,const RealFSHandle& h) : n(n), g(g), h(h) {}
		};
		map< LocalFd, fd_info > m;
};


#endif
