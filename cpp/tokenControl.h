#ifndef TOKENCONTROL_H
#define TOKENCONTROL_H


#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cassert>
#include <string>
#include "ids.h"

#define dprint(v) cerr << #v"=" << v << endl //;)
#define TOKEN_MICROSECONDS_TIME 5000000
using namespace std;

class token {
	public:	
		vector< pair<string,WorkerId> > getListaAltas();
		vector< pair<string,WorkerId> > getListaBajas();
		token(const vector< pair<string,WorkerId> >& lcp, vector< pair<string,WorkerId> >& ldp);
		token();
		#ifdef DEBUG_FLAG
		string say();
		#endif
	private:
		vector< pair<string,WorkerId> >  lc,ld;
};


class tokenControl {
	public:	
		int tickTime();
		void recvT(const token& nt);
		token getT();
		bool mustProc();
		tokenControl();
	private:
		token t;
		bool valid;
		clock_t trec;
};

#endif
