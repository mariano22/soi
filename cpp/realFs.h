#ifndef REALFS_H
#define REALFS_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <cassert>
#include <string>
using namespace std;

typedef FILE* RealFSHandle;

RealFSHandle openr(const string& fileName);
RealFSHandle openw(const string& fileName);
bool read(RealFSHandle h, int sz, string &data);
void write(RealFSHandle h, const string& txt);
void createf(const string& fileName);
void deletef(const string& fileName);
void close(RealFSHandle h);

#endif
