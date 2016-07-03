#ifndef SOCKAUX_H
#define SOCKAUX_H

#include <bits/stdc++.h>
#include <stdlib.h>
#include <sys/socket.h>       
#include <sys/types.h>       
#include <arpa/inet.h>      
#include <unistd.h>  
using namespace std;

bool getLineSocket(int sc, string &r);

#endif
