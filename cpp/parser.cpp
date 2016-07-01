#include "parser.h"

vector<string> parser(const string& input) {
	string aux;
    stringstream ss(input); 
    vector<string> res; 
    while (ss >> aux)
        res.push_back(aux);
	return res;
}

