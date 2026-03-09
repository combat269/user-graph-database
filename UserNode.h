#ifndef USER_NODE_H
#define USER_NODE_H

// DO NOT CHANGE THIS FILE

#include "Utility.h"

class UserNode {
public:
	string username;
	vector<string> follows;
	UserNode(string name) : username(name) { }
	UserNode() : username("") { }
};


#endif
