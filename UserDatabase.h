#ifndef USER_DATABASE_H
#define USER_DATABASE_H

#include "Utility.h"
#include "Exceptions.h"
#include "UserNode.h"

class UserDatabase {
private:
	// DO NOT CHANGE
	vector<UserNode*> _userTable;
	size_t _size;
	double _load_factor;
	UserNode* _dummy;
	// ADD YOUR HELPER FUCTIONS HERE

public:
	// DO NOT CHANGE
	inline void printDatabase() const {
		size_t max_length = 0U;
		for (size_t i = 0; i < capacity(); i++) {
			if (_userTable[i] == NULL)
				max_length = std::max(max_length, size_t(4U));
			else if (_userTable[i] == _dummy)
				max_length = std::max(max_length, size_t(5U));
			else
				max_length = std::max(max_length, _userTable[i]->username.size());
		}
		cout << string(max_length + 2U, '_') << endl;
		for (size_t i = 0; i < capacity(); i++) {
			if (_userTable[i] == NULL)
				cout << "|" << printStyle(string("NULL"), max_length) << "|" << endl;
			else if (_userTable[i] == _dummy)
				cout << "|" << printStyle(string("DUMMY"), max_length) << "|" << endl;
			else
				cout << "|" << printStyle(_userTable[i]->username, max_length) << "|" << endl;
		}
		cout << string(max_length + 2U, '_') << endl;
	}
	inline void printUserList(bool withCount = false) const {
		vector<string> uList = getUserList();
		for (size_t i = 0; i < uList.size(); i++) {
			cout << uList[i];
			if (withCount) {
				cout << "\t" << userFollowerCount(uList[i]) << "\t" << userFollowsCount(uList[i]);
			}
			cout << endl;
		}
	}

	// IMPLEMENT THESE
	UserDatabase();
	UserDatabase(size_t s, double load);
	~UserDatabase();

	size_t size() const;
	size_t capacity() const;
	bool isEmpty() const;

	void addUser(string username);
	void removeUser(string username);
	UserNode* getUser(string username) const;
	void resize(size_t size);

	void follow(string username1, string username2);
	void unfollow(string username1, string username2);
	bool userExists(string username) const;
	bool userFollows(string username1, string username2) const;
	size_t userFollowerCount(string username) const;
	size_t userFollowsCount(string username) const;
	vector<string> getUserList() const;
	void printAdjacencyMatrix() const;

	int BFS(string username1, string username2, bool printPath = true);
	int DFS(string username1, string username2, bool printPath = true);

	double averageBFS();
	double averageDFS();

	vector<string> getSharedNeighbourhood(string username1, string username2, size_t k);
};
#endif // !USER_DATABASE_H