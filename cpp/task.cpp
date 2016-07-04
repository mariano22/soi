#include "task.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

void myDelete(const string& s, void * ptr) {
	if (s=="fileName") {
		delete (string*) ptr;
	} else if (s=="idGlobal") {
		delete (GlobalId*) ptr;
	} else if (s=="GlobalFd") {
		delete (GlobalFd*) ptr;
	} else if (s=="cliente") {
		delete (ClientId*) ptr;
	} else if (s=="sizeTxt") {
		delete (int*) ptr;
	} else if (s=="strTxt") {
		delete (string*) ptr;
	} else if (s=="mensaje") {
		delete (mensaje *) ptr;
	} else if (s=="token") {
		delete (token *) ptr;
	} else assert(false);
}


taskName task::getTaskName() { return tN; }

void task::setTaskName(taskName ntN) { tN=ntN; }

string task::getFileName() { 
	map<string,void*>::iterator it = m.find("fileName");
	assert( it != m.end() ); 
	return *(string *)it->second;
}

void task::setFileName(const string& x) { 
	assert( m.find("fileName") == m.end() ); 
	m["fileName"]=(void*) new string(x);
}

GlobalId task::getIdGlobal() { 
	map<string,void*>::iterator it = m.find("idGlobal");
	assert( it != m.end() ); 
	return *(GlobalId *)it->second;
}

void task::setIdGlobal(const GlobalId& x) { 
	assert( m.find("idGlobal") == m.end() ); 
	m["idGlobal"]=(void*) new GlobalId(x);
}

GlobalFd task::getGlobalFd() { 
	map<string,void*>::iterator it = m.find("GlobalFd");
	assert( it != m.end() ); 
	return *(GlobalFd *)it->second;
}

void task::setGlobalFd(const GlobalFd& x) { 
	assert( m.find("GlobalFd") == m.end() ); 
	m["GlobalFd"]=(void*) new GlobalFd(x);
}

ClientId task::getCliente() { 
	map<string,void*>::iterator it = m.find("cliente");
	assert( it != m.end() ); 
	return *(ClientId *)it->second;
}

void task::setCliente(const ClientId& x) { 
	assert( m.find("cliente") == m.end() ); 
	m["cliente"]=(void*) new ClientId(x);
}

int task::getSizeTxt() { 
	map<string,void*>::iterator it = m.find("sizeTxt");
	assert( it != m.end() ); 
	return *(int *)it->second;
}

void task::setSizeTxt(const int& x) { 
	assert( m.find("sizeTxt") == m.end() ); 
	m["sizeTxt"]=(void*) new int(x);
}

string task::getStrTxt() { 
	map<string,void*>::iterator it = m.find("strTxt");
	assert( it != m.end() ); 
	return *(string *)it->second;
}

void task::setStrTxt(const string& x) { 
	assert( m.find("strTxt") == m.end() ); 
	m["strTxt"]=(void*) new string(x);
}

mensaje task::getMensaje() { 
	map<string,void*>::iterator it = m.find("mensaje");
	assert( it != m.end() ); 
	return *(mensaje *)it->second;
}

void task::setMensaje(const mensaje& x) { 
	assert( m.find("mensaje") == m.end() ); 
	m["mensaje"]=(void*) new mensaje(x);
}

token task::getToken() { 
	map<string,void*>::iterator it = m.find("token");
	assert( it != m.end() ); 
	return *(token *)it->second;
}

void task::setToken(const token& x) { 
	assert( m.find("token") == m.end() ); 
	m["token"]=(void*) new token(x);
}

void task::clear() {
	for (map<string,void*>::iterator it = m.begin() ; it != m.end() ; ++it) myDelete(it->first,it->second);
	m.clear();
}

task task::crear_workerDelete(const string& nameFile,const GlobalId& IdG) {
	task r; r.setTaskName(workerDelete);
	r.setFileName(nameFile);
	r.setIdGlobal(IdG);
	return r;
}

task task::crear_workerOpenRead(const string& nameFile,const GlobalId& IdG) {
	task r; r.setTaskName(workerOpenRead);
	r.setFileName(nameFile);
	r.setIdGlobal(IdG);
	return r;
}

task task::crear_workerOpenWrite(const string& nameFile,const GlobalId& IdG) {
	task r; r.setTaskName(workerOpenWrite);
	r.setFileName(nameFile);
	r.setIdGlobal(IdG);
	return r;
}

task task::crear_workerOpenSucc(const GlobalFd& gFd, const ClientId& cId) {
	task r; r.setTaskName(workerOpenSucc);
	r.setGlobalFd(gFd);
	r.setCliente(cId);
	return r;
}

task task::crear_workerWrite(const string& txt,const GlobalFd& gFd ,const GlobalId& IdG) {
	task r; r.setTaskName(workerWrite);
	r.setStrTxt(txt);
	r.setGlobalFd(gFd);
	r.setIdGlobal(IdG);
	return r;
}

task task::crear_workerRead(const int& Sz,const GlobalFd& gFd, const GlobalId& gId) {
	task r; r.setTaskName(workerRead);
	r.setSizeTxt(Sz);
	r.setGlobalFd(gFd);
	r.setIdGlobal(gId);
	return r;
}

task task::crear_workerClose(const GlobalFd& gFd, const GlobalId& gId) {
	task r; r.setTaskName(workerClose);
	r.setGlobalFd(gFd);
	r.setIdGlobal(gId);
	return r;
}

task task::crear_workerCloseSucc(const GlobalFd& gFd, const GlobalId& gId) {
	task r; r.setTaskName(workerCloseSucc);
	r.setGlobalFd(gFd);
	r.setIdGlobal(gId);
	return r;
}

task task::crear_workerCloseBye(const GlobalFd& gFd) {
	task r; r.setTaskName(workerCloseBye);
	r.setGlobalFd(gFd);
	return r;
}

task task::crear_workerSay(const ClientId& cId, const mensaje& m) {
	task r; r.setTaskName(workerSay);
	r.setCliente(cId);
	r.setMensaje(m);
	return r;
}

task task::crear_workerToken(const token& t) {
	task r; r.setTaskName(workerToken);
	r.setToken(t);
	return r;
}

pair<task,bool> task::fromUserData(const vector<string>& parsed_data, const ClientId& cId) {
	task r;
	if ( !parsed_data.size() ) return make_pair(r,false);
	if (parsed_data[0]=="CON") {
		r.setTaskName(userCon);
		if ( parsed_data.size()!=1 ) return make_pair(r,false);
	} else if (parsed_data[0]=="LSD") {
		r.setTaskName(userLsd);
		if ( parsed_data.size()!=1 ) return make_pair(r,false);
	} else if (parsed_data[0]=="DEL") {
		r.setTaskName(userDelete);
		if ( parsed_data.size()!=2 ) return make_pair(r,false);
		r.setFileName(parsed_data[1]);
	} else if (parsed_data[0]=="CRE") {
		r.setTaskName(userCreate);
		if ( parsed_data.size()!=2 ) return make_pair(r,false);
		r.setFileName(parsed_data[1]);
	} else if (parsed_data[0]=="OPNR") {
		r.setTaskName(userOpenRead);
		if ( parsed_data.size()!=2 ) return make_pair(r,false);
		r.setFileName(parsed_data[1]);
	} else if (parsed_data[0]=="OPNW") {
		r.setTaskName(userOpenWrite);
		if ( parsed_data.size()!=2 ) return make_pair(r,false);
		r.setFileName(parsed_data[1]);
	} else if (parsed_data[0]=="WRT") {
		r.setTaskName(userWrite);
		if ( parsed_data.size()!=6 || parsed_data[1]!="FD" || parsed_data[3]!="SIZE" ) return make_pair(r,false);
		r.setGlobalFd(stringToGlobalFd(parsed_data[2]));
		int size; stringstream ss(parsed_data[4]); ss >> size; 
		r.setSizeTxt(size);
		r.setStrTxt(parsed_data[5]);
	} else if (parsed_data[0]=="REA") {
		r.setTaskName(userRead);
		if ( parsed_data.size()!=5 || parsed_data[1]!="FD" || parsed_data[3]!="SIZE" ) return make_pair(r,false);
		r.setGlobalFd(stringToGlobalFd(parsed_data[2]));
		int size; stringstream ss(parsed_data[4]); ss >> size; 
		r.setSizeTxt(size);
	} else if (parsed_data[0]=="CLO") {
		r.setTaskName(userClose);
		if ( parsed_data.size()!=3 || parsed_data[1]!="FD") return make_pair(r,false);
		r.setGlobalFd(stringToGlobalFd(parsed_data[2]));
	} else if (parsed_data[0]=="BYE") {
		r.setTaskName(userBye);
		if ( parsed_data.size()!=1 ) return make_pair(r,false);
	} else return make_pair(r,false);
	r.setCliente(cId);
	return make_pair(r,true);
}
#ifdef DEBUG_FLAG
string taskNamePrint[] = { "userLsd", "userDelete", "userCreate", "userOpenRead", "userOpenWrite", "userWrite", "userRead", "userClose", "userBye", "userCon", "workerDelete", "workerOpenRead", "workerWrite", "workerRead", "workerOpenWrite", "workerSay", "workerOpenSucc", "workerClose", "workerCloseBye", "workerCloseSucc", "workerToken" } ;
string task::say() const {
	stringstream r;
	r << "task "+ taskNamePrint[tN] + " { " ;
	forall(it,m) { string s = it->fst;
		r << "(" + s + "," ;
		if (s=="fileName") {
			r  << (string*)it->snd;
		} else if (s=="idGlobal") {
			r << "(" << ((GlobalId*)it->snd)->fst << "," << ((GlobalId*)it->snd)->snd << ") ";
		} else if (s=="GlobalFd") {
			r << *(GlobalFd*)it->snd;
		} else if (s=="cliente") {
			r << *(ClientId*)it->snd;
		} else if (s=="sizeTxt") {
			r << *(int*)it->snd;
		} else if (s=="strTxt") {
			r << *(string*)it->snd;
		} else if (s=="mensaje") {
			r << ((mensaje *)it->snd)->say();
		} else if (s=="token") {
			r << ((token *)it->snd)->say();
		} else assert(false);
		r << ") // " ;
	}
	r << "}";
	return r.str();
}
#endif
