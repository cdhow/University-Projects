/**
 * Algorithm for K-Shortest-Paths BY Caleb Howard
 * - Run Dijkstra's algorithm on an adjacency matrix
 *   of the nodes
 * - The result is a vector of shortest distances
 *   from the source node to each node
 *
 * - Reverse the graph directions by transposing the adjacency matrix
 * - Run Dijkstra's algorithm on the transposed matrix with dst as src
 * - The result is a vector of shortest distances from each node
 *   to the destination node
 *
 * - By adding the corresponding values in these vectors together
 *   we will have a new vector of the shortest distance to that node from
 *   source plus the shortest distance from that node to the destination
 * - Taking the K minimum values from this vector will give us the shortest
 *   paths
 * - Some nodes that share the same path will have the same distance value
 *   therefore we load these values into a set, this will remove duplicates
 *   and sort the values
 *
 * Limitations:
 *      - We must calculate all k shortest paths where k=V
 *      - If two different paths have the same distance
 *        then one path will be ignored, it will not be listed
 *        as the second shortest.
 *
 * Note: I would usually split this program into multiple files, but
 * for simplicity for the marker I'll keep it all in the single file
 */

#include <iostream>
#include <vector>
#include <cfloat>
#include <fstream>
#include <tuple>
#include <chrono>
#include <cmath>
#include <set>
#include <algorithm>

// Loads the data from the specified input input file
// into an adjacency matrix and a transposed adjacency matrix,
// returns a tuple of, (src, dst, k, matrix. matrixT)
std::tuple<int, int, int, std::vector<std::vector<double>>,
std::vector<std::vector<double>>> load_matrix(std::string &infile)
{
    std::ifstream f(infile);
    if (!f.good()) std::cerr << "File does not exist: " + infile;
    std::string line;

    // Number of vertices and number of edges
    int n_v = 0, n_e = 0;

    // First line contains the n_v and n_e
    f >> n_v >> n_e;

    // Adjacency matrix of nodes from the input file
    std::vector<std::vector<double>> matrix(n_v, std::vector<double>(n_v, 0));
    // Transposed adjacency matrix of the nodes from input file
    std::vector<std::vector<double>> matrixT(n_v, std::vector<double>(n_v, 0));

    // node a, node b
    int a = 0, b = 0;
    // weight of edge between a, b
    double w = 0.0;
    // source and destination nodes
    int src = 0, dst = 0;
    // K shortest
    int k = 0;

    int lineNum = 1;
    while(lineNum <= n_e) {
        std::getline(f,line);
        f >> a >> b >> w;
        matrix[a][b] = w; // Load weight in matrix
        matrixT[b][a] = w; // Load weight in matrixT in reverse
        lineNum++;
    }

    // Last line contains src, dst and k
    f >> src >> dst >> k;

    return std::make_tuple(src, dst, k, matrix, matrixT);
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
double min_distance(std::vector<double> &dist, std::vector<int> &p)
{
    // Initialize min value
    double min = DBL_MAX;
    int minIndex;

    for (int v = 0; v < dist.size(); v++)
        if (!p[v] && dist[v] <= min)
            min = dist[v], minIndex = v;

    return minIndex;
}

// The Dijkstra's single source shortest path algorithm
// it takes an adjacency matrix, a source node and returns
// the distances the source to all other nodes.
// This method will be used on the original matrix to retrieve
// the distances from source to all other nodes.
// It will also be used on a transposed matrix, where the directions are
// reversed from the original matrix, this allows for us to
// set the destination as the source and run it in reverse to retrieve
// the distance from every node to the destination
std::vector<double> dijkstra(std::vector<std::vector<double>> &matrix, int src)
{
    // Output vector , dist[i] will hold the shortest path
    // cost from src to node i
    std::vector<double> dist(matrix.size(), DBL_MAX);

    // Unit vector, if node i is included in the shortest path or
    // the shortest distance from src to i is processed then
    // p[i] = 1 else = 0
    std::vector<int> p(matrix.size(), 0);

    // Distance of src node from itself is always 0
    dist[src] = 0.0;

    // Find shortest path for all vertices
    for (int count = 0; count < matrix.size() - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed
        double min_v = min_distance(dist, p);

        // Mark the node as processed
        p[min_v] = 1;

        // Update the dist value of the nodes adjacent to the processed node
        for (int v = 0; v < matrix.size(); v++) {
            // Update dist[v] only if it is not in p, there is an edge from
            // min_v to v, and total weight of path from src to  v through m is
            // smaller than the current value of dist[v]
            if (matrix[min_v][v] && dist[min_v] != DBL_MAX
                && dist[min_v] + matrix[min_v][v] < dist[v])
                dist[v] = dist[min_v] + matrix[min_v][v];
        }
    }

    // Return the list of distances from src to all nodes
    return dist;
}

void k_shortest_paths(std::vector<std::vector<double>> &matrix, std::vector<std::vector<double>> &matrixT,
                        int &src, int &dst, int &k)
{
    // Get the distances from src to all nodes
    std::vector<double> srcDist = dijkstra(matrix, src);

    // If the k=1 then the shortest path cost will be srcDst[dst]
    if (k==1) {
        std::cout << "K=1 shortest path: " << std::fixed << srcDist[dst] << std::endl;
        return;
    }

    // Get the distances from dst to all nodes by using the transposed matrix
    // and dst as source
    std::vector<double> dstDist = dijkstra(matrixT, dst);

    // Add the corresponding values in the vectors
    // Note: There are issues with decimal precision, truncating here helps
    std::transform(srcDist.begin(), srcDist.end(), dstDist.begin(), srcDist.begin(),
                   [](double &a, double &b) { return std::floor((a+b)*10000) / 10000; });

    // Nodes that are on the same path will have the same path distance
    // therefore we load the distances into a std::set, this will remove
    // duplicates and sort the paths
    // Note: if two different paths cost the same, one will be excluded
    // and not counted as the k-1th distance
    std::set<double> dists(srcDist.begin(), srcDist.end());



    // print k-shortest-distances // if k < dists size the loop will
    // terminate after length of dists
   int i=1;
    for (auto d : dists) {
        std::cout << "K=" << i << " shortest path: " << d << std::endl;
        if (i==k) break;
        i++;
    }
}

int main(int argc, char *argv[]) {
    std::string infile = "input_data.txt";

    // Optional file input argument
    if (argc > 2)
        infile = argv[1];

    // Get the adjacency matrix, the k-value, and the src and dest nodes
    std::vector<std::vector<double>> matrix;
    std::vector<std::vector<double>> matrixT;
    int src = 0, dst = 0, k = 0;
    std::tie(src, dst, k, matrix, matrixT) = load_matrix(infile);
    k=10;
    // Track algorithm run time
    auto startClock = std::chrono::high_resolution_clock::now();

    // Run K shortest Paths
    k_shortest_paths(matrix, matrixT, src, dst, k);

    auto endClock = std::chrono::high_resolution_clock::now();
    double microseconds = std::chrono::duration_cast<std::chrono::microseconds> (endClock - startClock).count();

    std::cout << "Time Elapsed: " <<  std::fixed << double(microseconds) / 1000000
    << " seconds" << std::endl;

    return 0;
}
