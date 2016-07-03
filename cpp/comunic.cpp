#include <bits/stdc++.h>
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

void enviarWorker(const WorkerId& dest,const task& t) {
	workerQueues[dest].push(t);
}
void responderCliente(const ClientId& dest,const mensaje& txt, WorkerScope* ws) {
	ws->find(dest)->push(txt);
}

int main() {
    freopen("input.in", "r", stdin);
    ios::sync_with_stdio(0);
    while(cin >> n){
    
    }
    return 0;
}
