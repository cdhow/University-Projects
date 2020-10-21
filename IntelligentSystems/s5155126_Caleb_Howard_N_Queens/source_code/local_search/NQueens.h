//
// Created by caleb on 12/3/20.
//

#ifndef N_QUEENS_BFS_NQUEENS_H
#define N_QUEENS_BFS_NQUEENS_H

#include <vector>
#include <iostream>
#include <random>


// Solver for the N-Queens problem using either the hill climbing algorithm or the Simulated Annealing alogrithm
class NQueens {
    int n_queens; // Number of queens/ board dimension ( N )

    // Method for creating the initial board configuration
    std::vector<int> createBoard();

    // Prints a given board
    static void printBoard(std::vector<int> &);

    // This method returns the cost (heuristic) of a given board
    static int cost(std::vector<int> &);

    // This method swaps the column positions of two queens
    void random_swap(std::vector<int> &);

    // Simple method to determine whether to run the algorithm (No solutions for N < 1, N=2 and N=3)
    bool is_valid_N_value();


public:

    // This method runs the algorithm for n queens
    // Algorithm is as follows
    // While cost != 0
    //      swap the two column positions of two random queens
    //      if the successor cost is less than the best cost then update the best board and best cost
    //      if the successor cost == the best cost for 10 runs, restart the board
    //      if the best cost == 0 then print board and return
    void solve_hill_climbing(const int &);

    // This method runs the algorithm for n queens
    // Algorithm is as follows
    // While temp > 0
    //      decrease temp by cooling factor
    //      swap the column positions of two queens
    //      if the successor cost is less than the best cost, update best cost and the best board
    //     else if the probability (cost difference / temp) gives true, then best board = successor board
    //     if best cost == 0 print board and return
    //
    // Takes n queens, a temperature and a cooling factor
    void solve_simulated_annealing(const int &, double, const double &);

};


#endif //N_QUEENS_BFS_NQUEENS_HH
