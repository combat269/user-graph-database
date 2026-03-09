#include "UserDatabase.h"
#include <algorithm> 
#include <queue>
#include <stack>


// IMPLEMENT THESE
UserDatabase::UserDatabase()
{
	// TODO
   _userTable.resize(11, NULL);
    _size = 0;
    _load_factor = 0.75;
    _dummy = new UserNode("dummy");
}

UserDatabase::UserDatabase(size_t s, double load)
{
	// TODO
 _userTable.resize(s, NULL);
    _size = 0;
    _load_factor = load;
    _dummy = new UserNode("dummy");
}

UserDatabase::~UserDatabase()
{
	// TODO
 for (size_t i = 0; i < _userTable.size(); i++) {
        if (_userTable[i] != NULL && _userTable[i] != _dummy) {
            delete _userTable[i];
        }
    }
    delete _dummy;
}

size_t UserDatabase::size() const
{
	// TODO
	  return _size;
}

size_t UserDatabase::capacity() const
{
	// TODO
	return _userTable.size();
}

bool UserDatabase::isEmpty() const
{
	// TODO
	 return _size == 0;
}

void UserDatabase::addUser(string username) {
	// TODO
     if (userExists(username)) {
        throw UsernameTakenException(username);
    }

    if (static_cast<double>(_size + 1) / capacity() > _load_factor) {
        resize(capacity() * 2 + 1);
    }

    size_t startPos = hashFunction(username, capacity());
    size_t pos = startPos;
    
    do {
        if (_userTable[pos] == NULL || _userTable[pos] == _dummy) {
            _userTable[pos] = new UserNode(username);
            _size++;
            return;
        }
        pos = (pos + 1) % capacity();
    } while (pos != startPos);

    
    resize(capacity() * 2 + 1);
    addUser(username);
}

void UserDatabase::removeUser(string username)
{
	// TODO
	if (!userExists(username)) {
        throw UserDoesNotExistException(username);
    }

    size_t startPos = hashFunction(username, capacity());
    size_t pos = startPos;

    
    do {
        if (_userTable[pos] != NULL && _userTable[pos]->username == username) {
            delete _userTable[pos];
            _userTable[pos] = _dummy; 
            _size--;

            
            for (size_t i = 0; i < _userTable.size(); ++i) {
                if (_userTable[i] != NULL && _userTable[i] != _dummy) {
                    vector<string>& follows = _userTable[i]->follows;
                    follows.erase(std::remove(follows.begin(), follows.end(), username), follows.end());
                }
            }
            return;
        }
        pos = (pos + 1) % capacity();
    } while (pos != startPos);
	
}

UserNode* UserDatabase::getUser(string username) const
{
	// TODO
	  size_t startPos = hashFunction(username, capacity());
    size_t pos = startPos;

    do {
        if (_userTable[pos] != NULL && _userTable[pos] != _dummy && _userTable[pos]->username == username) {
            return _userTable[pos];
        }
        pos = (pos + 1) % capacity();
    } while (_userTable[pos] != NULL && pos != startPos);

    throw UserDoesNotExistException(username);
//	return new UserNode();
}

void UserDatabase::resize(size_t size)
{
	// TODO
	 vector<UserNode*> oldTable = _userTable;
    _userTable.clear();
    _userTable.resize(size, NULL);
    _size = 0;

    for (size_t i = 0; i < oldTable.size(); ++i) {
        if (oldTable[i] != NULL && oldTable[i] != _dummy) {
            addUser(oldTable[i]->username);
            delete oldTable[i];
        }
    }
}

void UserDatabase::follow(string username1, string username2)
{
	// TODO
	 if (!userExists(username1)) {
        throw UserDoesNotExistException(username1);
    }
    if (!userExists(username2)) {
        throw UserDoesNotExistException(username2);
    }

    UserNode* user1 = getUser(username1);

    if (std::find(user1->follows.begin(), user1->follows.end(), username2) != user1->follows.end()) {
        throw RedundantActionException(username1, username2, true);
    }

    user1->follows.push_back(username2);
}

void UserDatabase::unfollow(string username1, string username2)
{
	// TODO
 if (!userExists(username1)) {
        throw UserDoesNotExistException(username1);
    }
    if (!userExists(username2)) {
        throw UserDoesNotExistException(username2);
    }

    UserNode* user1 = getUser(username1);

    vector<string>::iterator it = std::find(user1->follows.begin(), user1->follows.end(), username2);
    if (it == user1->follows.end()) {
        throw RedundantActionException(username1, username2, false);
    }

    user1->follows.erase(it);
}


bool UserDatabase::userExists(string username) const
{
	// TODO
 size_t startPos = hashFunction(username, capacity());
    size_t pos = startPos;
    size_t originalPos = pos;

    do {
        if (_userTable[pos] != NULL && _userTable[pos] != _dummy && _userTable[pos]->username == username) {
            return true;
        }
        pos = (pos + 1) % capacity();
    } while (pos != originalPos && _userTable[pos] != NULL);

    return false;
}

bool UserDatabase::userFollows(string username1, string username2) const
{
	// TODO
  if (!userExists(username1)) {
        throw UserDoesNotExistException(username1);
    }
    if (!userExists(username2)) {
        throw UserDoesNotExistException(username2);
    }

    UserNode* user1 = getUser(username1);
    vector<string>::const_iterator it = std::find(user1->follows.begin(), user1->follows.end(), username2);
    return it != user1->follows.end();
}

size_t UserDatabase::userFollowerCount(string username) const
{
	if (!userExists(username)) {
        throw UserDoesNotExistException(username);
    }

    size_t count = 0;
    for (size_t i = 0; i < _userTable.size(); ++i) {
        if (_userTable[i] != NULL && _userTable[i] != _dummy) {
            if (std::find(_userTable[i]->follows.begin(), _userTable[i]->follows.end(), username) != _userTable[i]->follows.end()) {
                ++count;
            }
        }
    }
    return count;
}

size_t UserDatabase::userFollowsCount(string username) const
{
	// TODO
if (!userExists(username)) {
        throw UserDoesNotExistException(username);
    }

    UserNode* user = getUser(username);
    return user->follows.size();
}

vector<string> UserDatabase::getUserList() const
{
	// TODO
 vector<string> userList;
    for (size_t i = 0; i < _userTable.size(); ++i) {
        if (_userTable[i] != NULL && _userTable[i] != _dummy) {
            userList.push_back(_userTable[i]->username);
        }
    }
    return userList;
}

void UserDatabase::printAdjacencyMatrix() const
{
	// TODO
vector<string> users = getUserList();
    size_t size = users.size();
    vector<vector<int> > matrix(size, vector<int>(size, 0));

    for (size_t i = 0; i < size; ++i) {
        UserNode* user = getUser(users[i]);
        for (size_t j = 0; j < user->follows.size(); ++j) {
            string follow = user->follows[j];
            vector<string>::iterator it = std::find(users.begin(), users.end(), follow);
            if (it != users.end()) {
                size_t index = std::distance(users.begin(), it);
                matrix[i][index] = 1;
            }
        }
    }

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int UserDatabase::BFS(string username1, string username2, bool printPath)
{
	// TODO

  if (!userExists(username1)) throw UserDoesNotExistException(username1);
    if (!userExists(username2)) throw UserDoesNotExistException(username2);

    std::vector<std::string> userList = getUserList();
    std::vector<int> visited(userList.size(), -1); 
    std::queue<std::string> q;
    std::string path;
    int distance = -1;

    int startIdx = -1;
    for (size_t i = 0; i < userList.size(); ++i) {
        if (userList[i] == username1) {
            startIdx = static_cast<int>(i);
            break;
        }
    }
    if (startIdx == -1) return -1;

    visited[startIdx] = 0; 
    q.push(username1);
    path = username1 + " ";

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        int currentIdx = -1;
        for (size_t i = 0; i < userList.size(); ++i) {
            if (userList[i] == current) {
                currentIdx = static_cast<int>(i);
                break;
            }
        }
        if (currentIdx == -1) continue;

        try {
            UserNode* currentUser = getUser(current);
            const std::vector<std::string>& follows = currentUser->follows;

            for (std::vector<std::string>::const_iterator it = follows.begin(); it != follows.end(); ++it) {
                const std::string& neighbor = *it;

                if (userExists(neighbor)) { // CRUCIAL CHECK
                    int neighborIdx = -1;
                    for (size_t j = 0; j < userList.size(); ++j) {
                        if (userList[j] == neighbor) {
                            neighborIdx = static_cast<int>(j);
                            break;
                        }
                    }
                    if (neighborIdx != -1 && visited[neighborIdx] == -1) {
                        visited[neighborIdx] = visited[currentIdx] + 1;
                        q.push(neighbor);
                        path += neighbor + " ";

                        if (neighbor == username2) {
                            distance = visited[neighborIdx];
                            if (printPath) std::cout << path << std::endl;
                            return distance;
                        }
                    }
                }
            }
        } catch (const UserDoesNotExistException& e) {
            
        }
    }

    if (printPath) std::cout << path << std::endl;
    return distance; 
//return 0;
}

int UserDatabase::DFS(string username1, string username2, bool printPath)
{
	// TODO
   if (!userExists(username1)) throw UserDoesNotExistException(username1);
    if (!userExists(username2)) throw UserDoesNotExistException(username2);

    vector<string> userList = getUserList();
    vector<bool> visited(userList.size(), false);
    std::stack<std::pair<string, int> > s;
    string path; 
    bool found = false;
    int finalDistance = -1;

    s.push(std::make_pair(username1, 0));
    path = username1; 

    while (!s.empty() && !found) {
        string current = s.top().first;
        int distance = s.top().second;
        s.pop();

        int currentIdx = -1;
        for (size_t i = 0; i < userList.size(); i++) {
            if (userList[i] == current) {
                currentIdx = i;
                break;
            }
        }

        if (currentIdx != -1 && !visited[currentIdx]) {
            visited[currentIdx] = true;
            
            if (current != username1) {  
                path += " " + current;
            }

            if (current == username2) {
                found = true;
                finalDistance = distance;
                break;
            }

            UserNode* currentUser = getUser(current);
            if (currentUser != _dummy) {
                const vector<string>& follows = currentUser->follows;
                
                for (int i = follows.size() - 1; i >= 0; i--) {
                    string neighbor = follows[i];
                    
                    int neighborIdx = -1;
                    for (size_t j = 0; j < userList.size(); j++) {
                        if (userList[j] == neighbor) {
                            neighborIdx = j;
                            break;
                        }
                    }

                    if (neighborIdx != -1 && !visited[neighborIdx]) {
                        s.push(std::make_pair(neighbor, distance + 1));
                    }
                }
            }
        }
    }

    if (printPath) {
        path += " ";  
        cout << path << endl;
    }

    return finalDistance;
  
}



double UserDatabase::averageBFS()
{
   vector<string> users = getUserList();
    double totalDistance = 0;
    int validPaths = 0;

    
    for (size_t i = 0; i < users.size(); ++i) {
        for (size_t j = 0; j < users.size(); ++j) {
            if (i != j) {  
                int distance = BFS(users[i], users[j], false);
                if (distance != -1) {  
                    totalDistance += distance;
                    validPaths++;
                }
            }
        }
    }

    return validPaths > 0 ? totalDistance / validPaths : 0;
}


double UserDatabase::averageDFS()
{
	// TODO
 vector<string> users = getUserList();
    double totalDistance = 0;
    int validPaths = 0;

   
    for (size_t i = 0; i < users.size(); ++i) {
        for (size_t j = 0; j < users.size(); ++j) {
            if (i != j) {  
                int distance = DFS(users[i], users[j], false);
                if (distance != -1) {  
                    totalDistance += distance;
                    validPaths++;
                }
            }
        }
    }

    return validPaths > 0 ? totalDistance / validPaths : 0;
}

vector<string> UserDatabase::getSharedNeighbourhood(string username1, string username2, size_t k)
{
	// TODO

    if (!userExists(username1)) {
        throw UserDoesNotExistException(username1);
    }
    if (!userExists(username2)) {
        throw UserDoesNotExistException(username2);
    }

    vector<string> sharedNeighborhood;
    vector<string> users = getUserList();

    for (size_t i = 0; i < users.size(); ++i) {
        string currentUser = users[i];

        if (currentUser == username1 || currentUser == username2) {
            continue;
        }

        int distanceToUser1 = BFS(username1, currentUser, false);
        int distanceToUser2 = BFS(username2, currentUser, false);

       
        if (distanceToUser1 != -1 && distanceToUser2 != -1 &&
            static_cast<size_t>(distanceToUser1) < k && static_cast<size_t>(distanceToUser2) < k) {
            sharedNeighborhood.push_back(currentUser);
        }
    }

    return sharedNeighborhood;
}

// ADD YOUR HELPER FUCTIONS HERE
