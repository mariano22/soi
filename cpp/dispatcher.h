#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <iostream>
#include <sys/socket.h>       
#include <sys/types.h>       
#include <arpa/inet.h>      
#include <string>        
#include <pthread.h>  
#include "mainWorker.h"
#include "mensaje.h"
#include "task.h"
#include "syncQueues.h"
#include "localConections.h"
#include "sockaux.h"
#include "parser.h"
using namespace std;

void launchDispatcher();

#endif
