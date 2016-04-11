#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

enum taskName { userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye, userCon, 
			    workerDelete, workerOpenRead, workerWrite, workerRead, workerOpenWrite, workerSay, workerOpenSucc, workerClose, 
				workerCloseBye, workerCloseSucc, workerToken } ;

typedef pair<int,int> GlobalId;
typedef int GlobalFd;
typedef int ClientId;
typedef string Mensaje; /* esta definicion de mensaje es dummy, debe ser borrada e importar el mensaje.h */

GlobalFd stringToGlobalFd(const string& sgFd) { // esta funcion debiera ir en ids
	stringstream ss(sgFd);
	GlobalFd res;
	ss >> res;
	return res;
}

typedef int WorkerId;
WorkerId noWorkerId=-1;
typedef int token; /* esta definicion de mensaje es dummy, debe ser borrada e importar el tokenControl.h */

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
		delete (Mensaje *) ptr;
	} else if (s=="token") {
		delete (token *) ptr;
	} else assert(false);
}

class task {
	public:	
		taskName getTaskName() { return tN; }
		void setTaskName(taskName ntN) { tN=ntN; }
		string getFileName() { 
			map<string,void*>::iterator it = m.find("fileName");
			assert( it != m.end() ); 
			return *(string *)it->second;
		}
		void setFileName(const string& x) { 
			assert( m.find("fileName") == m.end() ); 
			m["fileName"]=(void*) new string(x);
		}
		GlobalId getIdGlobal() { 
			map<string,void*>::iterator it = m.find("idGlobal");
			assert( it != m.end() ); 
			return *(GlobalId *)it->second;
		}
		void setIdGlobal(const GlobalId& x) { 
			assert( m.find("idGlobal") == m.end() ); 
			m["idGlobal"]=(void*) new GlobalId(x);
		}
		GlobalFd getGlobalFd() { 
			map<string,void*>::iterator it = m.find("GlobalFd");
			assert( it != m.end() ); 
			return *(GlobalFd *)it->second;
		}
		void setGlobalFd(const GlobalFd& x) { 
			assert( m.find("GlobalFd") == m.end() ); 
			m["GlobalFd"]=(void*) new GlobalFd(x);
		}
		ClientId getCliente() { 
			map<string,void*>::iterator it = m.find("cliente");
			assert( it != m.end() ); 
			return *(ClientId *)it->second;
		}
		void setCliente(const ClientId& x) { 
			assert( m.find("cliente") == m.end() ); 
			m["cliente"]=(void*) new ClientId(x);
		}
		int getSizeTxt() { 
			map<string,void*>::iterator it = m.find("sizeTxt");
			assert( it != m.end() ); 
			return *(int *)it->second;
		}
		void setSizeTxt(const int& x) { 
			assert( m.find("sizeTxt") == m.end() ); 
			m["sizeTxt"]=(void*) new int(x);
		}
		string getStrTxt() { 
			map<string,void*>::iterator it = m.find("strTxt");
			assert( it != m.end() ); 
			return *(string *)it->second;
		}
		void setStrTxt(const string& x) { 
			assert( m.find("strTxt") == m.end() ); 
			m["strTxt"]=(void*) new string(x);
		}
		Mensaje getMensaje() { 
			map<string,void*>::iterator it = m.find("mensaje");
			assert( it != m.end() ); 
			return *(Mensaje *)it->second;
		}
		void setMensaje(const Mensaje& x) { 
			assert( m.find("mensaje") == m.end() ); 
			m["mensaje"]=(void*) new Mensaje(x);
		}
		token getToken() { 
			map<string,void*>::iterator it = m.find("token");
			assert( it != m.end() ); 
			return *(token *)it->second;
		}
		void setToken(const token& x) { 
			assert( m.find("token") == m.end() ); 
			m["token"]=(void*) new token(x);
		}
		
		void clear() {
			for (map<string,void*>::iterator it = m.begin() ; it != m.end() ; ++it) myDelete(it->first,it->second);
			m.clear();
		}
		
		static task crear_workerDelete(const string& nameFile,const GlobalId& IdG) {
			task r; r.setTaskName(workerDelete);
			r.setFileName(nameFile);
			r.setIdGlobal(IdG);
			return r;
		}
		static task crear_workerOpenRead(const string& nameFile,const GlobalId& IdG) {
			task r; r.setTaskName(workerOpenRead);
			r.setFileName(nameFile);
			r.setIdGlobal(IdG);
			return r;
		}
		static task crear_workerOpenWrite(const string& nameFile,const GlobalId& IdG) {
			task r; r.setTaskName(workerOpenWrite);
			r.setFileName(nameFile);
			r.setIdGlobal(IdG);
			return r;
		}
		static task crear_workerOpenSucc(const GlobalFd& gFd, const ClientId& cId) {
			task r; r.setTaskName(workerOpenSucc);
			r.setGlobalFd(gFd);
			r.setCliente(cId);
			return r;
		}
		static task crear_workerWrite(const string& nameFile,const GlobalId& IdG) {
			task r; r.setTaskName(workerWrite);
			r.setFileName(nameFile);
			r.setIdGlobal(IdG);
			return r;
		}
		static task crear_workerRead(const int& Sz,const GlobalFd& gFd, const GlobalId& gId) {
			task r; r.setTaskName(workerRead);
			r.setGlobalFd(gFd);
			r.setIdGlobal(gId);
			return r;
		}
		static task crear_workerClose(const GlobalFd& gFd, const GlobalId& gId) {
			task r; r.setTaskName(workerClose);
			r.setGlobalFd(gFd);
			r.setIdGlobal(gId);
			return r;
		}
		static task crear_workerCloseSucc(const GlobalFd& gFd, const GlobalId& gId) {
			task r; r.setTaskName(workerCloseSucc);
			r.setGlobalFd(gFd);
			r.setIdGlobal(gId);
			return r;
		}
		static task crear_workerCloseBye(const GlobalFd& gFd) {
			task r; r.setTaskName(workerCloseBye);
			r.setGlobalFd(gFd);
			return r;
		}
		static task crear_workerSay(const ClientId& cId, const Mensaje& m) {
			task r; r.setTaskName(workerSay);
			r.setCliente(cId);
			r.setMensaje(m);
			return r;
		}
		static task crear_workerToken(const token& t) {
			task r; r.setTaskName(workerToken);
			r.setToken(t);
			return r;
		}
		static pair<task,bool> fromUserData(const vector<string>& parsed_data, const ClientId& cId) {
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

	private:
		taskName tN;
		map<string,void*> m;
};

int main() {
	// just for testing purpose
	//~ string a("  WRT FD  10  SIZE   20   asdasdasd");
	//~ vector<string> r = parser(a);
	//~ pair<task,bool> t = task::fromUserData(r,0);
	//~ if (t.second) {
		//~ cout << t.first.getCliente() << endl;
		//~ cout << t.first.getGlobalFd() << endl;
		//~ cout << t.first.getSizeTxt() << endl;
		//~ cout << t.first.getStrTxt() << endl;
	//~ }
	return 0;
}
