#include "realFs.h"
#define WORKER_FS_PATH "./fs/"

RealFSHandle openr(string fileName) {
	fileName = WORKER_FS_PATH + fileName;
	FILE* result=fopen (fileName.c_str(),"r");
	assert(result!=NULL);
	return result;
}
RealFSHandle openw(string fileName) {
	fileName = WORKER_FS_PATH + fileName;
	FILE* result=fopen (fileName.c_str(),"w");
	assert(result!=NULL);
	return result;
}
bool read(RealFSHandle h, int sz, string &data) {
	if (feof(h)) return false;
	char *buffer = (char*) malloc (sizeof(char)*(sz+1));
	fread ( buffer, 1, sz, h );
	data=string(buffer);
	free(buffer);
	return true;
}
void write(RealFSHandle h, const string& txt) {
	fwrite (txt.c_str() , sizeof(char), txt.size(), h);
	fflush(h);
}
void createf(string fileName) {
	fileName = WORKER_FS_PATH + fileName;
	fclose(fopen(fileName.c_str(),"w"));
}
void deletef(string fileName) {
	fileName = WORKER_FS_PATH + fileName;
	remove(fileName.c_str());
}
void close(RealFSHandle h) {
	fclose(h);
}

