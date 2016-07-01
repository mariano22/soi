#ifndef LOCALFILES_H
#define LOCALFILES_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

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
