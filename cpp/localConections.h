#ifndef LOCALCONECTIONS_H
#define LOCALCONECTIONS_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include "ids.h"
#include "mensaje.h"
#include "syncQueues.h"

class localConections {
	public:
		syncQueues<mensaje>* find(const ClientId& cId);
		void delC(const ClientId& cId);
		void newC(const ClientId& cId, syncQueues<mensaje>* qp);
	private:
		map< ClientId,syncQueues<mensaje>* > m;
};

#endif
