#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

typedef FILE* RealFSHandle;

RealFSHandle openr(const string& fileName) {
	return fopen (fileName.c_str(),"r");
}
RealFSHandle openw(const string& fileName) {
	return fopen (fileName.c_str(),"w");
}
bool read(RealFSHandle h, int sz, string &data) {
	if (feof(h)) return false;
	char *buffer = (char*) malloc (sizeof(char)*(sz+1));
	fread ( buffer, 1, sz, h );
	data=string(buffer);
	free(buffer);
	return false;
}
void write(RealFSHandle h, const string& txt) {
	fwrite (txt.c_str() , sizeof(char), txt.size(), h);
}
void createf(const string& fileName) {
	fclose(fopen(fileName.c_str(),"w"));
}
void deletef(const string& fileName) {
	remove(fileName.c_str());
}
void close(RealFSHandle h) {
	fclose(h);
}

int main() {
	return 0;
}
