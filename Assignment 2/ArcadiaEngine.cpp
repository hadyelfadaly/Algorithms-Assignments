#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <functional>

const int TABLE_SIZE = 101;

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable
{

private:

    //to hold data
    struct Entry
    {

        int id;
        string name;
        bool isOccupied = false; //track if slot  empty or full

    };
    vector<Entry> table;

    int h1(int key) //multiplication method - floor( tableSize * ( (key * A) % 1 ) )
    {
        const double A = 0.6180339887; //a constant number such that its between 0 and 1, 0.618 is a common choice among programmers
        double val = key * A, fractionalPart = val - floor(val); //same as to % 1 as % operator doesnt work on double data types in c++

        return floor(TABLE_SIZE * fractionalPart);

    }
    int h2(int key)  //double hashing - P - (key % P), where P is prime < tableSize
    {

        int P = 97; //biggest prime less than 101

        return P - (key % P);

    }

public:

    //passing fixed 101 size
    ConcretePlayerTable() : table(TABLE_SIZE) {}

    void insert(int playerID, string name) override
    {

        int index = h1(playerID), step = h2(playerID), i = 0;

        //while slot  is occupied
        while(table[index].isOccupied)
        {

            //if ID  exist,  just update  name
            if(table[index].id == playerID)
            {

                table[index].name = name;

                return;

            }

            i++;
            index = (index + step) % TABLE_SIZE; //formula: (h1 + i*h2) % tableSize (the double hashing)

            //if table full or loop infinite
            if(i > TABLE_SIZE)
            {

                cout << "Error: Table is full" << endl;

                return;

            }

        }

        //found empty slot
        table[index].id = playerID;
        table[index].name = name;
        table[index].isOccupied = true;

    }
    string search(int playerID) override
    {

        int index = h1(playerID), step = h2(playerID), i = 0;

        //until we find the player or an empty slot
        while(table[index].isOccupied)
        {

            if(table[index].id == playerID) return table[index].name;

            i++;
            index = (index + step) % TABLE_SIZE;

            if(i > TABLE_SIZE) break;

        }

        // Return "" if player not found
        return "";

    }

};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard
{

private:

    struct node
    {

        int playerID;
        int score;
        vector<node*> next; //vector of forward pointers for skip list levels

    };

    node *header; //head of skip list
    int maxLevel , currentLevel; //max levels, current highest level
    float probability; //for random level generation

    //helper function to generate random level for new node
    int randomLevel()
    {

        int lvl = 0;

        //The rand() function in C++ generates a random integer between 0 and RAND_MAX,
        //so dividing rand() by RAND_MAX gives a fraction between 0 and 1.
        //We keep increasing the level while the generated random number is less than the probability
        //and we don't exceed the maximum level of the skip list (lvl < maxLevel - 1)
        while(((double) rand() / RAND_MAX) < probability && lvl < maxLevel - 1) lvl++;

        return lvl;

    }

public:

    ConcreteLeaderboard()
    {

        header = new node(); //initialize header node
        header->score = INT_MAX; //highest possible score at header
        header->playerID = -1; //dummy ID
        maxLevel = 7; //ceil of log2(TABLE_SIZE), since the max number of players is 101, max level can be 7
        currentLevel = 0;
        header->next.resize(maxLevel, nullptr);
        probability = 0.5; //common choice for skip lists

    }
    //helper function to search for a player by score
    vector<node*> searchByScore(int score)
    {

        node* current = header;
        vector<node*> result; //to hold nodes with the exact score

        //start from highest level
        for(int i = currentLevel; i >= 0; i--)
        {

            //move forward while next node's score is greater than target score
            while(current->next[i] != nullptr && current->next[i]->score > score) current = current->next[i];

        }

        //move to next node at level 0
        current = current->next[0];

        //check if we found the node with the exact score
        while(current != nullptr && current->score == score) result.push_back(current), current = current->next[0];

        return result; //not found

    }
    //helper function to search for a player by ID
    node* searchByID(int playerID)
    {

        //since our skip list is sorted by score not player id, we will traverse level 0 linearly to find the playerID

        node* current = header->next[0]; //start from first real node at level 0

        //move forward while next node's ID is less than target ID
        while(current != nullptr)
        {

            if(current->playerID == playerID) return current; //found the target

            current = current->next[0];

        }

        return nullptr; //not found

    }
    void addScore(int playerID, int score) override
    {

        //update array to hold insertion position at each level
        vector<node*> update(maxLevel, nullptr);
        node* current = header;

        //find insertion position at each level
        for(int i = currentLevel; i >= 0; i--)
        {

            //while the next node in this level is not null and its score is higher than the score we are inserting
            //then move forward in this level, if 2 scores are equal, we use playerID to maintain consistent ordering
            while(current->next[i] != nullptr && (current->next[i]->score > score
            || (current->next[i]->score == score && current->next[i]->playerID < playerID))) current = current->next[i];

            update[i] = current; //store the last node before the insertion point at this level

        }

        int nodeLevel = randomLevel(); //get random level for new node

        if(nodeLevel > currentLevel)
        {

            //if new node's level is higher than current highest level, update the update array for those levels
            for(int i = currentLevel + 1; i <= nodeLevel; i++) update[i] = header; //point to header for new levels

            currentLevel = nodeLevel; //update current highest level

        }

        //create new node
        node* newNode = new node();
        newNode->playerID = playerID;
        newNode->score = score;
        newNode->next.resize(nodeLevel+1, nullptr);

        //insert new node by adjusting forward pointers at each level
        for(int i = 0; i <= nodeLevel; i++)
        {

            newNode->next[i] = update[i]->next[i]; //new node points to the next node at this level
            update[i]->next[i] = newNode; //previous node points to new node

        }

    }
    void removePlayer(int playerID) override
    {

        vector<node*> update(maxLevel, nullptr);
        node* current = header, *targetNode = searchByID(playerID); //get target node to delete
        int targetScore;

        if(targetNode != nullptr) targetScore = targetNode->score; //get target score that we will use to delete
        else return; //player not found, nothing to delete

        //find position at each level
        for(int i = currentLevel; i >= 0; i--)
        {

            //traverse until we find the node we will delete
            while(current->next[i] != nullptr && (current->next[i]->score > targetScore
                  || (current->next[i]->score == targetScore && current->next[i]->playerID < playerID)))  current = current->next[i];

            update[i] = current;

        }

        //verify we found the target
        node* toDelete = update[0]->next[0];
        if(toDelete == nullptr || toDelete != targetNode) return;

        //remove target node by adjusting forward pointers at each level
        for(int i = 0; i <= currentLevel; i++) if(update[i]->next[i] == targetNode) update[i]->next[i] = targetNode->next[i];

        delete targetNode; //free memory

        //adjust current level if necessary after deletion
        while(currentLevel > 0 && header->next[currentLevel] == nullptr) currentLevel--;

    }
    vector<int> getTopN(int n) override
    {

        vector<int> topPlayers;

        node* current = header->next[0]; //start from the first real node at level 0

        while(current != nullptr && n-- > 0)
        {

            topPlayers.push_back(current->playerID); //add playerID to result
            current = current->next[0]; //move to next node

        }

        return topPlayers;

    }

    //optional destructor to free memory to avoid memory leaks
    ~ConcreteLeaderboard()
    {

        node* current = header;

        while(current != nullptr)
        {

            node* next = current->next[0];

            delete current;

            current = next;

        }

    }

};

// --- 3. AuctionTree (Red-Black Tree) ---

class ConcreteAuctionTree : public AuctionTree
{

private:

    struct RBNode
    {

        int itemID, price;
        bool color;              //colors : red = true, black = false
        RBNode* left, *right, *parent;

    };

    RBNode* root, *NIL;

    //helper function for creating new node
    RBNode* createNode(int id, int price)
    {

        RBNode* n = new RBNode();
        n->itemID = id;
        n->price = price;
        n->color = true;         //red
        n->left = n->right = n->parent = NIL;

        return n;

    }

    //helper rotation functions
    void leftRotate(RBNode* x)
    {

        RBNode* y = x->right;
        x->right = y->left;

        if(y->left != NIL) y->left->parent = x;

        y->parent = x->parent;

        if(x->parent == NIL) root = y; //rotates with the root
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;

    }
    void rightRotate(RBNode* x)
    {

        RBNode* y = x->left;
        x->left = y->right;

        if(y->right != NIL) y->right->parent = x;

        y->parent = x->parent;

        if(x->parent == NIL) root = y; //rotates with the root
        else if(x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;

        y->right = x;
        x->parent = y;

    }

    //helper function for fixing tree after the insertion
    void fixInsert(RBNode* z)
    {

        while(z->parent->color == true) //parent is red
        {

            //if parent is on left
            if(z->parent == z->parent->parent->left)
            {

                RBNode* y = z->parent->parent->right; //uncle

                //case 1: uncle is red
                if(y->color == true)
                {

                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;

                }
                else //uncle black
                {

                    //case 2: uncle black and z and its ancestors make triangle shape
                    if(z == z->parent->right)
                    {

                        z = z->parent;

                        leftRotate(z);

                    }

                    //case 3: uncle black and z and its ancestors make a line
                    z->parent->color = false;
                    z->parent->parent->color = true;

                    rightRotate(z->parent->parent);

                }
            }
            else //if parent is on right
            {

                RBNode* y = z->parent->parent->left; //uncle

                //case1: uncle is red
                if(y->color == true)
                {

                    z->parent->color = false;
                    y->color = false;
                    z->parent->parent->color = true;
                    z = z->parent->parent;

                }
                else //uncle black
                {

                    //case 2: uncle black and z and its ancestors make triangle shape
                    if(z == z->parent->left)
                    {

                        z = z->parent;

                        rightRotate(z);

                    }

                    //case 3: uncle black and z and its ancestors make a line
                    z->parent->color = false;
                    z->parent->parent->color = true;

                    leftRotate(z->parent->parent);

                }

            }

        }

        root->color = false; //root must always be black

    }

    //helper function for replacing nodes (with its subtrees) before deleting
    void transplant(RBNode* u, RBNode* v)
    {

        if(u->parent == NIL) root = v;
        else if(u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;

        v->parent = u->parent;

    }

    //helper function to get successor
    RBNode* minimum(RBNode* x)
    {
        while (x->left != NIL) x = x->left;

        return x;

    }

    //helper function fixing after the deletion
    void fixDelete(RBNode* x)
    {

        while(x != root && x->color == false)
        {

            if(x == x->parent->left)
            {

                RBNode* w = x->parent->right; //sibling

                //case 1: sibling is red
                if(w->color == true)
                {

                    w->color = false;
                    x->parent->color = true;

                    leftRotate(x->parent);

                    w = x->parent->right;

                }
                if(w->left->color == false && w->right->color == false) //both nephews black
                {

                    w->color = true;
                    x = x->parent;

                }
                else
                {

                    //far child is red or both red
                    if(w->right->color == false)
                    {

                        w->left->color = false;
                        w->color = true;

                        rightRotate(w);

                        w = x->parent->right;

                    }

                    w->color = x->parent->color;
                    x->parent->color = false;
                    w->right->color = false;

                    leftRotate(x->parent);

                    x = root;

                }
            }
            else //uncle on the left
            {

                RBNode* w = x->parent->left;

                //uncle is red
                if(w->color == true)
                {

                    w->color = false;
                    x->parent->color = true;

                    rightRotate(x->parent);

                    w = x->parent->left;

                }
                if(w->right->color == false && w->left->color == false) //both nephews black
                {

                    w->color = true;
                    x = x->parent;

                }
                else
                {

                    //far child is red or both red
                    if(w->left->color == false)
                    {

                        w->right->color = false;
                        w->color = true;

                        leftRotate(w);

                        w = x->parent->left;

                    }

                    w->color = x->parent->color;
                    x->parent->color = false;
                    w->left->color = false;

                    rightRotate(x->parent);

                    x = root;

                }

            }

        }

        x->color = false;

    }

    //helper function for searching by id
    RBNode* searchByID(RBNode* node, int id)
    {

        if(node == NIL) return NIL;
        if(node->itemID == id) return node;

        RBNode* left = searchByID(node->left, id);
        if(left != NIL) return left;

        return searchByID(node->right, id);

    }

    //helper function to recursively delete nodes
    void destroyTree(RBNode* node)
    {
        if(node != NIL)
        {

            destroyTree(node->left);
            destroyTree(node->right);

            delete node;

        }

    }

public:

    ConcreteAuctionTree()
    {

        NIL = new RBNode();
        NIL->color = false;   //black
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;

    }

    void insertItem(int itemID, int price) override
    {

        RBNode* z = createNode(itemID, price), *y = NIL, *x = root;

        while(x != NIL)
        {

            y = x;
            if(price < x->price || (price == x->price && itemID < x->itemID)) x = x->left;
            else x = x->right;

        }

        z->parent = y;

        if(y == NIL) root = z; //no parent so its the root
        else if(price < y->price || (price == y->price && itemID < y->itemID)) y->left = z;
        else y->right = z;

        fixInsert(z);

    }
    void deleteItem(int itemID) override
    {

        RBNode* z = searchByID(root, itemID);

        //we couldnt find the node with the given id
        if(z == NIL) return;

        RBNode* y = z, *x;
        bool yOriginalColor = y->color;

        if(z->left == NIL) //whether it has one child or it is a leaf node
        {

            x = z->right;

            transplant(z, z->right);

        }
        else if(z->right == NIL)
        {

            x = z->left;

            transplant(z, z->left);

        }
        else //it has 2 children
        {

            y = minimum(z->right); //successor
            yOriginalColor = y->color;
            x = y->right;

            if(y->parent == z) x->parent = y;
            else
            {

                transplant(y, y->right);

                y->right = z->right;
                y->right->parent = y;

            }

            transplant(z, y);

            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;

        }

        delete z;

        if(yOriginalColor == false) fixDelete(x);
    }

    ~ConcreteAuctionTree()
    {

        destroyTree(root);

        delete NIL;

    }

};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins)
{

    //calc the total sum of all coins
    int total = 0;
    for(int c : coins) total += c;

    //half of total sum
    int target = total / 2;

    //dp[s] = true if a subset with sum 's' is possible
    vector<bool> dp(target + 1, false);

    dp[0] = true; //base case: sum 0 is always achievable

    //for each coin, update dp from target down to coin value
    for(int coin : coins)
    {

        for(int s = target; s >= coin; s--)
            dp[s] = dp[s] || dp[s - coin]; //either keep current sum or include this coin

    }

    //find the largest achievable sum <= target
    int best = 0;

    for(int s = target; s >= 0; s--)
    {

        if(dp[s])
        {

            best = s;

            break;

        }

    }

    //min difference between the two subsets
    return total - 2 * best;

}
int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items)
{

    //dp[w] = max value we can achieve with weight w
    vector<int> dp(capacity + 1, 0);

    //loop through each item
    for(auto& item : items)
    {

        int weight = item.first, value = item.second;

        //traverse from high to low weight to avoid reuse the same item
        for(int w = capacity; w >= weight; w--) dp[w] = max(dp[w], dp[w - weight] + value);

    }

    //the max value achievable within the capacity
    return dp[capacity];

}
long long InventorySystem::countStringPossibilities(string s)
{

    const long long MOD = 1000000007;
    int n = s.length();

    //dp[i] = number of ways to decode the first i characters
    vector<long long> dp(n + 1, 0);

    dp[0] = 1; // Base case: empty string has 1 way

    for(int i = 1; i <= n; i++)
    {

        //take single char
        dp[i] = dp[i - 1];

        //check double subs
        if(i >= 2)
        {

            if((s[i - 2] == 'u' && s[i - 1] == 'u') || (s[i - 2] == 'n' && s[i - 1] == 'n'))
                dp[i] = (dp[i] + dp[i - 2]) % MOD;

        }

    }

    //return total number of possible decodings
    return dp[n];

}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest)
{
    //if source equals destination, a path exists without moving
    if(source == dest) return true;

    //create an adjacency list for n nodes: adj[u] will store all neighbors of u
    vector<vector<int>> adj(n);

    //build the undirected graph from the edge list
    for(auto &e : edges)
    {

        //read the endpoints of the edge
        int u = e[0], v = e[1];

        //add v as a neighbor of u
        adj[u].push_back(v);
        adj[v].push_back(u); //add u as a neighbor of v (because edges are bidirectional)

    }

    //visited[i] tells us if city i has already been discovered in BFS
    vector<bool> visited(n, false);
    queue<int> q; //queue used by BFS to process nodes in "layers"

    //mark the source as visited so we don’t push it multiple times
    visited[source] = true;

    //start BFS from the source node
    q.push(source);

    //continue BFS while there are nodes to process
    while (!q.empty())
    {
        //take the front node from the queue (current BFS node)
        int cur = q.front();

        //remove it from the queue
        q.pop();

        //explore all neighbors of the current node
        for(int neighbour : adj[cur])
        {

            //if neighbor is not visited, we can discover it now
            if(!visited[neighbour])
            {

                //if this neighbor is the destination, a path exists
                if(neighbour == dest) return true;

                //mark neighbor visited to avoid revisiting
                visited[neighbour] = true;

                //add neighbor to queue for later exploration
                q.push(neighbour);

            }

        }

    }

    //if BFS ends without reaching dest, no path exists
    return false;

}

//helper function returns the set representative (root) of x with path compression
int findSet(int x, vector<int>& parent, vector<int>& rank)
{

    //if x is the representative of its set
    if(parent[x] == x) return x;

    //path compression attach x directly to the root
    parent[x] = findSet(parent[x], parent, rank);

    return parent[x];

}

//helper function to union two sets; returns true if merged, false if already same set
bool unionSet(int a, int b, vector<int>& parent, vector<int>& rank)
{

    //find the set representatives (roots) of a and b
    a = findSet(a, parent, rank);
    b = findSet(b, parent, rank);

    //already in the same set → cycle
    if(a == b) return false;

    //union by rank
    if(rank[a] < rank[b]) swap(a, b);

    parent[b] = a;

    if(rank[a] == rank[b]) rank[a]++;

    return true;

}
long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData)
{

    //define an Edge struct to store the MST edge data.
    struct Edge
    {

        long long cost;   //the computed bribe cost of this road
        int u, v;         //endpoints of the road

    };

    //store all computed edges here
    vector<Edge> edges;

    //reserve space for up to m edges to avoid repeated reallocations (performance)
    edges.reserve(m);

    //convert each roadData entry into an Edge with a computed cost.
    for(auto &r : roadData)
    {

        //extract endpoints.
        int u = r[0], v = r[1];

        //extract gold and silver costs from input
        long long gold = r[2], silver = r[3];

        //compute final bribe cost = goldCost*goldRate + silverCost*silverRate
        long long cost = gold * goldRate + silver * silverRate;

        //store the computed edge
        edges.push_back({cost, u, v});

    }

    //sort edges by increasing cost (required for Kruskals algorithm)
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return a.cost < b.cost;
    });

    //DSU arrays: parent[i] is the representative parent of i, rank is for balancing unions
    vector<int> parent(n), rank(n, 0);

    //initialize parent[i] = i (each node starts in its own set)
    for(int i = 0; i < n; i++) parent[i] = i;

    //total will hold the MST total cost
    long long total = 0;
    int used = 0; //used counts how many edges have been added to the MST

    //kruskal: take edges from smallest to largest, add if they connect two components
    for(auto &e : edges)
    {

        //if this edge connects two different components, accept it into MST
        if(unionSet(e.u, e.v, parent, rank))
        {
            //add its cost to the MST total
            total += e.cost;
            used++; //count it as used

            //MST is complete when we have n-1 edges
            if(used == n - 1) break;

        }

    }

    //if we used fewer than n-1 edges, the graph is disconnected => MST impossible
    return (used == n - 1) ? total : -1;

}
string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads)
{

    //INF represents "no connection" divide max to avoid overflow when adding
    const long long INF = LLONG_MAX / 4;

    //dist matrix: dist[i][j] starts as INF, then becomes shortest distance after Floyd–Warshall
    vector<vector<long long>> dist(n, vector<long long>(n, INF));

    //distance from any node to itself is always 0.
    for(int i = 0; i < n; i++) dist[i][i] = 0;

    //set direct road distances (undirected) and keep minimum if duplicates exist
    for(auto &r : roads)
    {

        //read endpoints and weight.
        int u = r[0], v = r[1];
        long long w = r[2];

        //update direct distances using min in case multiple roads connect same pair
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);

    }

    //Floyd–Warshall triple loop:
    //k is the intermediate vertex we are allowed to use
    for (int k = 0; k < n; k++)
    {

        //i is the start vertex.
        for (int i = 0; i < n; i++)
        {

            //if i cannot reach k, skip because i->k->j is impossible
            if(dist[i][k] >= INF) continue;

            //j is the end vertex.
            for(int j = 0; j < n; j++)
            {

                //if k cannot reach j, skip because i->k->j is impossible.
                if(dist[k][j] >= INF) continue;

                //relaxation: improve dist[i][j] via k if it's shorter.
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

            }

        }

    }

    //sum all shortest distances for unique unordered pairs (i < j).
    //we use __int128 to avoid overflow if the sum becomes large.
    __int128 sum = 0;

    //loop over all pairs i<j, only add if reachable
    for(int i = 0; i < n; i++) for(int j = i + 1; j < n; j++) if (dist[i][j] < INF) sum += dist[i][j];

    //if sum is 0, binary representation is "0"
    if(sum == 0) return "0";

    //build binary representation (we extract bits from least significant to most)
    string bin;

    //while sum still has bits.
    while(sum > 0)
    {

        //append '1' if the last bit is 1, else '0'.
        bin.push_back((sum & 1) ? '1' : '0');

        //shift right by 1 (divide by 2).
        sum >>= 1;

    }

    //we built bits in reverse order (LSB first), so reverse to correct order
    reverse(bin.begin(), bin.end());

    //return binary string.
    return bin;

}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n)
{

    //count frequencies of each task using a fixed array to store counts.
    map<char, int> counts;

    for(char t : tasks) counts[t]++;

    //push frequencies into a Max-Heap (Priority Queue)
    //the heap allows us to greedily select the task with the most work left.
    priority_queue<int> pq;

    for(auto const& [key, val] : counts) pq.push(val);

    int totalIntervals = 0;

    while(!pq.empty())
    {

        vector<int> temp_store;
        int cycle = n + 1; //the size of one full execution + cooling frame
        int processed_in_cycle = 0;

        //try to fill the current cycle with up to (n+1) distinct tasks
        for(int i = 0; i < cycle; i++)
        {

            if(!pq.empty())
            {

                temp_store.push_back(pq.top());
                pq.pop();

                processed_in_cycle++;

            }

        }

        //add the processed tasks back to heap if they still have remaining instances
        for(int count : temp_store) if(--count > 0) pq.push(count);

        //if the heap is NOT empty after this cycle, it means we have more work to do
        //we must pay the full cost of the cycle (n + 1), including idles if we couldn't fill it
        //if the heap is empty, we are done, so we only add the actual tasks processed
        if(!pq.empty()) totalIntervals += cycle;
        else totalIntervals += processed_in_cycle;

    }

    return totalIntervals;

}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C"
{

    PlayerTable* createPlayerTable()
    {return new ConcretePlayerTable();}
    Leaderboard* createLeaderboard()
    {return new ConcreteLeaderboard();}
    AuctionTree* createAuctionTree()
    {return new ConcreteAuctionTree();}

}
