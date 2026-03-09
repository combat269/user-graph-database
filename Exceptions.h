#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// DO NOT CHANGE THIS FILE

#include "Utility.h"


struct UsernameTakenException : runtime_error
{
	UsernameTakenException(const string& username) : runtime_error(username + string(" is already taken.")) {}
};

struct UserDoesNotExistException : runtime_error {
	UserDoesNotExistException(const string& username) : std::runtime_error(username + string(" does not exist.")) {}
};

struct RedundantActionException : std::runtime_error {
	RedundantActionException(const string& username1, const string& username2, const bool& follow) : runtime_error((follow ? (username1 + string(" already follows ") + username2 + string(".")) : (username1 + string(" already does not follow ") + username2) + string("."))) {}
};

 
#endif // !EXCEPTIONS_H
