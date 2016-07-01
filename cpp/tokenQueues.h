#ifndef tokenQueues_H
#define tokenQueues_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <string>
#include "ids.h"

using namespace std;

class tokenQueues {
	public:
		void newCreate(const string& fileName, ClientId cid);
		void newDelete(const string& fileName);
		vector< pair<string,ClientId> > getCreates();
		vector< string > getDeletes();
	private:
		map< string,ClientId > c;
		set<string> d;
};

#endif
