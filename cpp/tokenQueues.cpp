#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cassert>
using namespace std;

//~ % Funciones:
//~ % newCreate : String, ClientId -> ok (agrega a la lista de candidatos a crearse, el cliente es para saber a quien responder)
//~ % newDelete : String -> ok (agrega a la lista de bajas a informarse)
//~ % getCreates : () -> [{String,ClientId}] (extrae la lista de creaciones candidatas)
//~ % getDeletes : () -> [String] (extrae la lista de bajas a informarse)

typedef int ClientId;

class tokenQueues {
	public:
		void newCreate(const string& fileName, ClientId cid) {
			assert( c.find(fileName) == c.end() );
			c[fileName]=cid;
		}
		void newDelete(const string& fileName) {
			assert( d.find(fileName) == d.end() );
			d.insert(fileName);
		}
		vector< pair<string,ClientId> > getCreates() {
			vector< pair<string,ClientId> > result;
			for(map<string,ClientId>::iterator it = c.begin() ; it != c.end() ; ++it) 
				result.push_back(*it);
			c.clear();
			return result;
		}
		vector< string > getDeletes() {
			vector< string > result;
			for(set<string>::iterator it = d.begin() ; it != d.end() ; ++it) 
				result.push_back(*it);
			c.clear();
			return result;
		}
	private:
		map< string,ClientId > c;
		set<string> d;
};

int main() {
	return 0;
}
