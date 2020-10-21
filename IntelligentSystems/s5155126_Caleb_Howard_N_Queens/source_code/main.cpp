#include <iostream>
#include <chrono>
#include "BFS/BFS_NQueens.h"
#include "local_search/NQueens.h"

/*
 * #######################################################################################
 *                                 N-Queens Problem
 * This program can use the following algorithms to solve the N-Queens problem:
 * - a breadth first search implementation to find ALL solutions
 * - a Hill Climbing implementation to find a SINGLE solution
 * - a Simulated Annealing algorithm to find a SINGLE solution
 *
 * Created by ~ Caleb Howard
 *#######################################################################################
 */

// Takes an option and the N value and runs an algorithm based on the option, returns the algorithm run time
double run(const uint32_t &opt, int &n) {

    // Track time
    auto startClock = std::chrono::high_resolution_clock::now();

    switch (opt) {
        case 1:
            solve_bfs(n);
            break;
        case 2:
            NQueens().solve_hill_climbing(n);
            break;
        case 3:
            NQueens().solve_simulated_annealing(n, 4000, 0.99);
            break;
        default:
            std::cout << opt << " is not a valid option, please try again.\n" << std::endl;
            return 0;
    }

    auto endClock = std::chrono::high_resolution_clock::now();
    double microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>
                    (endClock - startClock).count();

    std::cout << "Algorithm run time: " << double(microseconds) / 1000000<< " seconds.\n"  << std::endl;

    return microseconds;
}

int main() {

    std::cout<< "Welcome to the N-Queens Solver ~ Created by Caleb Howard.\n" << std::endl;

    // Program loop
    int opt = 0;
    while (true)
    {
        // N-Queens
        int n;

        // Range of N-Queens to run algorithm against
        int n_min; int n_max;

        // Choose Algorithm
        std::cout << "Choose from the following algorithms:\n1. BFS (all solutions).\n2. Hill Climbing (single solution).\n"
                     "3. Simulated Annealing (single solution).\n4. Exit program." << std::endl;
        std::cin >> opt;

        if (opt == 4) {
            std::cout<< "\nBye!";
            break;
        }

        // Option to run for a range of N values
        std::cout << "Run Algorithm for a range of N values? (y/n)" << std::endl;
        char run_range; std::cin >> run_range;

        if (run_range == 'y' || run_range == 'Y')
        {
            std::cout<< "\nInput the N range lower bound: "; std::cin >> n_min;
            std::cout<< "\nInput the N range upper bound: "; std::cin >> n_max;

            // Run the algorithm for N = n_min to N = n_max
            double net_runtime = 0;
            for (int nval = n_min; nval<=n_max; nval++)
                net_runtime += run(opt, nval);

            // Print the net runtime of solutions N=n_min to N=n_max
            std::cout << "Combined runtime for range: ["<<n_min<<".."<<n_max<<"]: "
            << net_runtime/ 1000000 << " seconds\n" << std::endl;

        } else if (run_range == 'n' || run_range == 'N')
        {
            // Choose the number of Queens
            std::cout<< "\nHow many Queens should be solved?: "; std::cin >> n;
            std::cout << std::endl;

            // run algorithm
            run(opt, n);
        }
        else
        {
            std::cout << "Error: invalid choice.\n" << std::endl;
        }

    }

    return 0;
}