#include "tokenControl.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second
#define TOKEN_MILLISECONDS_TIME 5000

vector< pair<string,WorkerId> > token::getListaAltas() {
	vector< pair<string,WorkerId> > res = lc;
	lc.clear();
	return res;
}

vector< pair<string,WorkerId> > token::getListaBajas() {
	vector< pair<string,WorkerId> > res = ld;
	ld.clear();
	return res;
}

token::token() {}
token::token(const vector< pair<string,WorkerId> >& lcp, vector< pair<string,WorkerId> >& ldp) {
	lc = lcp;
	ld = ldp;
}



int tokenControl::tickTime() { return TOKEN_MILLISECONDS_TIME; }

void tokenControl::recvT(const token& nt) {
	assert(!valid);
	valid=true;
	clock_gettime(CLOCK_MONOTONIC, &trec);
	t=nt;
}
token tokenControl::getT() {
	assert(valid);
	valid=false;
	return t;
}
bool tokenControl::mustProc() {
	if (!valid) return false;
	struct timespec actual; clock_gettime(CLOCK_MONOTONIC, &actual);
	double seconds = (actual.tv_sec - trec.tv_sec);
	seconds += (actual.tv_nsec - trec.tv_nsec) / 1000000000.0;
	//~ DEBUG
	//~ cout << "diff " << seconds  << endl;
	return seconds > double(TOKEN_MILLISECONDS_TIME) / 1000. ;
}

tokenControl::tokenControl() { valid=false; }

#ifdef DEBUG_FLAG
string token::say() {
	stringstream r;
	r << "Token:" << endl << "Lista de altas: " ;
	forall(it,lc) r << "(" << it->fst << " , " << it->snd << ") ; " ;
	r << endl << "Lista de bajas: ";
	forall(it,ld) r << "(" << it->fst << " , " << it->snd << ") ; " ;
	r << endl;
	
	return r.str();
}
#endif

#ifdef DEBUG_FLAG
string tokenControl::say()  {
	stringstream r;
	if (!valid) r << "token invalid";
	else {
		r <<"token: " << t.say();
	}
	return r.str();
}
#endif

