#ifndef LOCALCONECTIONS_H
#define LOCALCONECTIONS_H

#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include "ids.h"

class localConections {
	public:
		ClientId find(const WorkerId& wId);
		void delC(const WorkerId& wId);
		void newC(const WorkerId& wId, ClientId id);
	private:
		map< ClientId,WorkerId > m;
};

#endif
