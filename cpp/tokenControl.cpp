#include "tokenControl.h"
#include <sstream>
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define fst first
#define snd second

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



int tokenControl::tickTime() { return TOKEN_MICROSECONDS_TIME; }

void tokenControl::recvT(const token& nt) {
	assert(!valid);
	valid=true;
	trec=clock();
	t=nt;
}
token tokenControl::getT() {
	assert(valid);
	valid=false;
	return t;
}
bool tokenControl::mustProc() {
	if (!valid) return false;
	double seconds = double(clock()-trec) / CLOCKS_PER_SEC;
	return seconds > double(TOKEN_MICROSECONDS_TIME) / 1000000. ;
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

