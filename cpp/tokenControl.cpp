#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cassert>
#define dprint(v) cerr << #v"=" << v << endl //;)
#define TOKEN_MICROSECONDS_TIME 1000
using namespace std;

typedef int WorkerId;
WorkerId noWorkerId=-1;

class token {
	public:	
		vector< pair<string,WorkerId> > getListaAltas() {
			vector< pair<string,WorkerId> > res = lc;
			lc.clear();
			return res;
		}
		vector< pair<string,WorkerId> > getListaBajas() {
			vector< pair<string,WorkerId> > res = ld;
			ld.clear();
			return res;
		}
		token() {}
		token(const vector< pair<string,WorkerId> >& lcp, vector< pair<string,WorkerId> >& ldp) {
			lc = lcp;
			ld = ldp;
		}
	private:
		vector< pair<string,WorkerId> >  lc,ld;
};


class tokenControl {
	public:	
		int tickTime() { return TOKEN_MICROSECONDS_TIME; }
		void recvT(const token& nt) {
			assert(!valid);
			valid=true;
			trec=clock();
			t=nt;
		}
		token getT() {
			assert(valid);
			valid=false;
			return t;
		}
		bool mustProc() {
			assert(valid);
			double seconds = double(clock()-trec) / CLOCKS_PER_SEC;
			return seconds > double(TOKEN_MICROSECONDS_TIME) / 1000000. ;
		}
		tokenControl() { valid=false; }
	private:
		token t;
		bool valid;
		clock_t trec;
};

int main() {
	// just for testing purpose
	token t;
	tokenControl tc;
	for(int i=0 ; true ; i++) {
		tc.recvT(t);
		while(!tc.mustProc());
		t=tc.getT();
		cout << "tick " << i << "\n";
		//~ assert(false);
	}
	return 0;
}
