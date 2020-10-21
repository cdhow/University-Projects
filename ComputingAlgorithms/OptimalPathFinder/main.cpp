#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <map>
#include <tuple>
#include <stack>
#include <chrono>

// This program uses a hash table to store the adjacency matrix of costs
// between two nodes we use the hash value of x and y in some manner to access this cost.
// Another map is used to stop a vector of neighbours for a given node. This is more efficient
// then search through all nodes and checking if another node is the current nodes neighbour
// The simple pseudo explanation is as follows:
// lookup currentNode's neighourNodes in neighbourMap table, iterator over neighbourNodes vector,
// access cost with costMap(currentNode, NeighbourNode)

// ------ Functor Classes and Types ------

// Hash functor used to store a pair of edges as the key, and the cost between them as the value in a hashtable
typedef const std::pair<uint32_t, uint32_t> key_p;
struct pair_hash : public std::unary_function<key_p, std::size_t>
{
    std::size_t operator ()(key_p &k) const
    {
        return (k.first + 1) ^ (k.second + 1);
    }
};
typedef std::unordered_map<const key_p, uint32_t, pair_hash> map_p;

// Map that contains an edge as a key and a vector of neighbours as the value
typedef std::map<const uint32_t, std::vector<uint32_t>> map_n;

// ------- Program Functions -------------

// Load data
void generateMaps(map_p &costMap, map_n &neighMap, uint32_t &numNodes)
{
    std::ifstream file;
    file.open("data.txt", std::ios::in);

    // First value in file in number of nodes
    file >> numNodes;

    // Populate hash table
    // y axis (rows)
    for (uint32_t y = 0; y < numNodes; y++) {
        // x axis (columns)
        for (uint32_t x = 0; x < numNodes; x++) {
            uint32_t cost; file >> cost;
            // Don't add 0 cost (non links)
            if (cost != 0) {
                costMap[key_p(x, y)] = cost;
                neighMap[y].push_back(x);
            }

        }
    }
}

// Main program loop
void DFS(map_p &costMap, map_n &neighMap, uint32_t &numNodes, const uint32_t &src, const uint32_t &target)
{
    // vector of nodes from source to target, initially null
    std::vector<uint32_t> parent(numNodes);

    // Vector of node costs from source to target, initially infinity
    std::vector<uint32_t> cost(numNodes, 999999999);

    // Stack stores unexplored branches
    std::stack<uint32_t> stack;

    uint32_t numPops = 0, numPush = 0;

    uint32_t curr;
    cost[src] = 0;
    stack.push(src);
    // Run operation
    while (!stack.empty())
    {
        curr = stack.top();
        stack.pop();
        numPops++;
        // If current != target loop all neighbours linked to current
        if (curr != target)
            for (auto neigh : neighMap[curr]) {
                // Get the cost of current to neighbour
                auto cost_it = costMap.find(key_p(curr, neigh));
                // If the cost of the neighbour is greater than cost of current
                if (cost[neigh] > (cost[curr] + cost_it->second)
                && (cost[curr] + cost_it->second) < cost[target]) {
                    cost[neigh] = (cost[curr] + cost_it->second);
                    parent[neigh] = curr;
                    stack.push(neigh);
                    numPush++;
                }
            }
    }

    // Get path w/ size statistic
    std::stack<uint32_t> path;
    path.push(target);
    uint32_t idx = target, size = 0;
    while (idx != src)
    {
        size += (costMap.find(key_p(idx, parent[idx])))->second;
        path.push(parent[idx]);
        idx = parent[idx];
    }

    // Print path w/ statistics
    uint32_t hops = 0;
    std::cout << "Optimal Path: ";
    while (!path.empty())
    {
        hops++;
        std::cout << path.top() << " ";
        path.pop();
    }
    std::cout << std::endl;
    std::cout << "Shortest number of hops: " << hops << "\nPath size: " << size << std::endl;
    std::cout << "Number of pops: " << numPops << "\nNumber of pushes: " << numPush << std::endl;
}

int main() {

    // Number of nodes
    uint32_t numNodes;

    // Map of node costs
    map_p costMap;

    // Map of lists of each nodes neighbour
    map_n neighMap;

    // Create cost hashtable and neighbour map of neighbour vectors
    generateMaps(costMap, neighMap, numNodes);

    // Clock DFS time
    auto start = std::chrono::high_resolution_clock::now();

    // Run program
    DFS(costMap, neighMap, numNodes, 0, numNodes-1);

    // Stop the clock
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Print time
    std::cout << "Path find speed (microseconds) : " << duration.count() << std::endl;

    // Will be Used to show that path is correct when demonstrating to lecturer.
//    for (auto i : costMap)
//        std::cout << i.first.first << " " << i.first.second << " " << i.second << std::endl;
    return 0;
}