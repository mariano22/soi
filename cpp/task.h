#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include "tokenControl.h"
#include "mensaje.h"
#include "ids.h"

using namespace std;

enum taskName { userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye, userCon, workerDelete, workerOpenRead, workerWrite, workerRead, workerOpenWrite, workerSay, workerOpenSucc, workerClose, workerCloseBye, workerCloseSucc, workerToken } ;

#ifdef DEBUG_FLAG
extern string taskNamePrint[];
#endif

void myDelete(const string& s, void * ptr) ;

class task {
	public:	
		taskName getTaskName();
		void setTaskName(taskName ntN);
		string getFileName();
		void setFileName(const string& x);
		GlobalId getIdGlobal();
		void setIdGlobal(const GlobalId& x);
		GlobalFd getGlobalFd();
		void setGlobalFd(const GlobalFd& x);
		ClientId getCliente();
		void setCliente(const ClientId& x);
		int getSizeTxt();
		void setSizeTxt(const int& x);
		string getStrTxt();
		void setStrTxt(const string& x);
		mensaje getMensaje();
		void setMensaje(const mensaje& x);
		token getToken();
		void setToken(const token& x);
		
		void clear();		
		static task crear_workerDelete(const string& nameFile,const GlobalId& IdG);
		static task crear_workerOpenRead(const string& nameFile,const GlobalId& IdG);
		static task crear_workerOpenWrite(const string& nameFile,const GlobalId& IdG);
		static task crear_workerOpenSucc(const GlobalFd& gFd, const ClientId& cId);
		static task crear_workerWrite(const string& txt, const GlobalFd& gFd, const GlobalId& IdG);
		static task crear_workerRead(const int& Sz,const GlobalFd& gFd, const GlobalId& gId);
		static task crear_workerClose(const GlobalFd& gFd, const GlobalId& gId);
		static task crear_workerCloseSucc(const GlobalFd& gFd, const GlobalId& gId);
		static task crear_workerCloseBye(const GlobalFd& gFd);
		static task crear_workerSay(const ClientId& cId, const mensaje& m);
		static task crear_workerToken(const token& t);
		static pair<task,bool> fromUserData(const vector<string>& parsed_data, const ClientId& cId);
		
		#ifdef DEBUG_FLAG
		string say() const;
		#endif
	private:
		taskName tN;
		map<string,void*> m;
};

#endif
