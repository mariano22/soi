#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#define forall(it,v) for(typeof(v.begin()) it=v.begin();it!=v.end();++it)
using namespace std;

vector<string> parser(const string& input) {
	string aux;
    stringstream ss(input); 
    vector<string> res; 
    while (ss >> aux)
        res.push_back(aux);
	return res;
}
/*
int main() {
	//~ string a("  REA FD  10  SIZE   20  ");
	//~ vector<string> r = parser(a);
	//~ forall(it,r) cout << "MyWord: " << *it << endl;
	return 0;
}
*/
