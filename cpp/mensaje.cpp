#include "mensaje.h"

mensaje mensaje::mOk() { 
	mensaje r; 
	r.setHeader("OK");
	return r;
}

mensaje mensaje::mErr() { 
	mensaje r; 
	r.setHeader("ERROR");
	return r;
}

mensaje mensaje::archivoNoExiste() { 
	mensaje r = mErr();
	r.addArg("notExist");
	return r;
}

mensaje mensaje::archivoOcupado() { 
	mensaje r = mErr();
	r.addArg("Ocuppied");
	return r;
}

mensaje mensaje::archivoExistente() { 
	mensaje r = mErr();
	r.addArg("Exist");
	return r;
}

mensaje mensaje::permisoDenegado() { 
	mensaje r = mErr();
	r.addArg("AccessDenied");
	return r;
}

mensaje mensaje::finDeArchivo() { 
	mensaje r = mErr();
	r.addArg("EOF");
	return r;
}


mensaje mensaje::archivoCreado() { 
	mensaje r = mOk();
	r.addArg("Create");
	return r;
}

mensaje mensaje::archivoBorrado() { 
	mensaje r = mOk();
	r.addArg("Deleted");
	return r;
}

mensaje mensaje::archivoCerrado() { 
	mensaje r = mOk();
	r.addArg("Close");
	return r;
}

mensaje mensaje::archivoWriteSucc() { 
	mensaje r = mOk();
	r.addArg("Writed");
	return r;
}

mensaje mensaje::archivoReadSucc(const string& Txt) { 
	mensaje r = mOk();
	r.addArg(Txt);
	return r;
}

mensaje mensaje::archivoAbierto(const GlobalFd& gFd) { 
	mensaje r = mOk();
	r.addArg(globalFdToString(gFd));
	return r;
}

mensaje mensaje::coneccionEstablecida(const ClientId& cId) { 
	mensaje r = mOk();
	r.addArg(clientIdToString(cId));
	return r;
}

string mensaje::say() {
	string res = header + (int(args.size()) ? " " : "\n");
	for(int i=0 ; i < int(args.size()) ; i++) {
		res = res + args[i] + ( i==int(args.size())-1 ? "\n" : " " );
	}
	return res;
}




void mensaje::setHeader(const string& h) { header = h; }
void mensaje::addArg(const string& arg) { args.push_back(arg); }
