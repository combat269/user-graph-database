#ifndef UTILITY_H
#define UTILITY_H

// DO NOT CHANGE THIS FILE

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <stdlib.h>

using std::string;
using std::vector;
using std::runtime_error;
using std::cout;
using std::endl;
using std::queue;

inline string printStyle(const string& s, size_t max_length) {
	size_t buffer = max_length - s.size();
	string retval("");
	if (buffer % 2U != 0) {
		retval += string(" ");
		buffer--;
	}
	retval += string(buffer / 2U, ' ');
	retval += s;
	retval += string(buffer / 2U, ' ');
	return retval;
}

inline size_t hashFunction(const string& username, size_t capacity) {
	size_t retval = 0;
	for (size_t i = 0; i < username.length(); i++) {
		retval = 37 * retval + size_t(username[i]);
		retval %= capacity;
	}

	return retval;
}

#endif // !UTILITY_H

