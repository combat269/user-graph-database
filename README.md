# UserDatabase — Hash Table Social Network Engine

A social network backend implemented from scratch in C++, combining an **open-addressing hash table** for O(1) user lookup with a **directed follow graph** for relationship traversal. Supports BFS/DFS pathfinding, average distance computation, and shared neighbourhood detection across the user graph.

---

## What It Does

Users are stored in a hash table with linear probing. Each user node also holds a directed adjacency list of who they follow, forming a graph layered on top of the hash map. The system supports the full lifecycle of a social network — adding and removing users, managing follow relationships, and running graph algorithms to analyze the network's structure.

---

## Data Structure Design

### Hash Table

- **Open addressing with linear probing** for collision resolution
- **Dummy node sentinel** to preserve probe chain integrity after deletions — deleted slots are marked `DUMMY` rather than `NULL`, so lookups don't terminate prematurely
- **Dynamic resizing** — when the load factor threshold is exceeded, the table doubles in capacity (`2n + 1`) and all live entries are rehashed
- **Polynomial rolling hash** on the username string: `hash = 37 * hash + char[i]`

### User Graph

- Each `UserNode` contains a `vector<string>` adjacency list of followed usernames
- The graph is **directed** — following is not mutual
- When a user is removed, their username is also purged from all other users' adjacency lists to keep the graph consistent

---

## API

### User Management

```cpp
UserDatabase db;                  // default: capacity 11, load factor 0.75
UserDatabase db(size, load);      // custom capacity and load factor

db.addUser("alice");              // throws UsernameTakenException if duplicate
db.removeUser("alice");           // throws UserDoesNotExistException if missing
db.getUser("alice");              // returns UserNode*, throws if not found
db.userExists("alice");           // bool
db.size();                        // number of live users
db.capacity();                    // current table size
```

### Follow Graph

```cpp
db.follow("alice", "bob");        // throws RedundantActionException if already follows
db.unfollow("alice", "bob");      // throws RedundantActionException if not following
db.userFollows("alice", "bob");   // bool
db.userFollowerCount("alice");    // how many users follow alice
db.userFollowsCount("alice");     // how many users alice follows
db.printAdjacencyMatrix();        // prints N×N follow matrix
```

### Graph Traversal

```cpp
// Returns hop distance between two users (-1 if unreachable)
int dist = db.BFS("alice", "bob");        // breadth-first, shortest path
int dist = db.DFS("alice", "bob");        // depth-first

// Average path length across all reachable user pairs
double avg = db.averageBFS();
double avg = db.averageDFS();

// Users reachable from BOTH alice and bob within k hops
vector<string> shared = db.getSharedNeighbourhood("alice", "bob", k);
```

---

## Implementation Notes

**Why dummy nodes?** With open addressing, simply nulling a deleted slot breaks the probe chain — later lookups would stop too early and falsely report a user as missing. The dummy sentinel keeps the chain intact while marking the slot as reusable for future insertions.

**Why `2n + 1` resize?** Odd table sizes improve hash distribution by reducing the chance of clustering at even-indexed slots for even-valued hash results.

**Follow removal on user delete:** When a user is removed, the code performs a full table scan to erase that username from every other user's `follows` vector. This keeps the graph consistent — BFS/DFS won't traverse stale edges to deleted nodes.

**BFS vs DFS distance:** BFS guarantees the shortest path. DFS returns the depth at which the target is first found along one traversal path — not necessarily the minimum. `averageBFS()` and `averageDFS()` both run all-pairs traversal, so they can be compared to observe how path estimates differ between the two strategies.

---

## Exception Handling

| Exception                   | Trigger                                                                 |
| --------------------------- | ----------------------------------------------------------------------- |
| `UsernameTakenException`    | `addUser` called with existing username                                 |
| `UserDoesNotExistException` | Any operation on a non-existent username                                |
| `RedundantActionException`  | Following an already-followed user, or unfollowing someone not followed |

---

## Files

```
├── UserDatabase.cpp    ← Full implementation
├── UserDatabase.h      ← Class definition
├── UserNode.h          ← Node: username + follows list
├── Utility.h           ← Hash function + print helpers
├── Exceptions.h        ← Custom exception types
└── main.cpp            ← Entry point
```

---

## Build

```bash
g++ -std=c++11 -o userdatabase main.cpp UserDatabase.cpp
./userdatabase
```

---

## Language & Tools

- **C++11**
- No external libraries — STL only (`vector`, `queue`, `stack`, `algorithm`)
