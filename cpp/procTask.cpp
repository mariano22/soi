#include <bits/stdc++.h>
#include "comunic.h"
#include "mensaje.h"

using namespace std;
#define dprint(v) cerr << #v"=" << v << endl //;)
#define forr(i,a,b) for(int i=(a); i<(b); i++)
#define forn(i,n) forr(i,0,n)
#define dforn(i,n) for(int i=n-1; i>=0; i--)
#define forall(it,v) for(auto it=v.begin();it!=v.end();++it)
#define sz(c) ((int)c.size())
#define zero(v) memset(v, 0, sizeof(v))
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int,int> ii;

const int MAXN=100100;
int n;

enum taskName { userLsd, userDelete, userCreate, userOpenRead, userOpenWrite, userWrite, userRead, userClose, userBye, userCon, 
			    workerDelete, workerOpenRead, workerWrite, workerRead, workerOpenWrite, workerSay, workerOpenSucc, workerClose, 
				workerCloseBye, workerCloseSucc, workerToken } ;

struct task {
	taskName getTaskName() const {return userLsd;}
};

void procTask(WorkerScope *who,const task& t) {
	switch (t.getTaskName()) {
		case userLsd:
				ClientId c=getCliente().t;
				responderCliente(c,coneccionEstablecida(c));
		break;
		case userDelete:
		break;
		case userCreate:
		break;
		case userOpenRead:
		break;
		case userOpenWrite:
		break;
		case userWrite:
		break;
		case userRead:
		break;
		case userClose:
		break;
		case userBye:
		break;
		case userCon:
		break;
		case workerDelete:
		break;
		case workerOpenRead:
		break;
		case workerWrite:
		break;
		case workerRead:
		break;
		case workerOpenWrite:
		break;
		case workerSay:
		break;
		case workerOpenSucc:
		break;
		case workerClose:
		break;
		case workerCloseBye:
		break;
		case workerCloseSucc:
		break;
		case workerToken:
		break;
	}
}


int main() {
    
    return 0;
}
