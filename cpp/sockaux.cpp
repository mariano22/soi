#include "sockaux.h"
using namespace std;
bool getLineSocket(int sc, string &r) {
	r.clear(); char bf;
	do {
		if ( read(sc,&bf,1) <=0 ) {
			close(sc);
			return true;
		}
		r.push_back(bf);
	} while (bf!='\n' && bf!='\0');
	return false;
}

