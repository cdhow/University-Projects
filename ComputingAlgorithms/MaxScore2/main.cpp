#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include "PriorityQueue.h"

// This class contains the data for a single test case that is loaded in from the input file
class TestCase {
public:
    // Maximum number of turns allowed per round
    int turns;

    // Max Heap priority queue
    PriorityQueue pQueue;

    // Coin toss value, (Rusty's turn is tails)
    bool isTails;

    // Time it takes to push values onto the queue
    double microseconds;

};

// Takes an input filename and returns a List of TestCases
std::list<TestCase> get_file_data(const std::string &filename)
{
    std::list<TestCase> testCases;

    std::ifstream infile;
    infile.open(filename);

    // A line
    std::string str;

    // First line is the number of test cases
    std::getline(infile, str);

    // Temp testCase
    TestCase temp = TestCase();

    // Loop line by line
    int line_num = 1;
    while (std::getline(infile, str))
    {
        // Track the time it takes to push values onto pQueue
        auto startClock = std::chrono::high_resolution_clock::now();

        // Create a string stream from the current line
        std::string buf; // buffer string
        std::istringstream iss(str);

        if (line_num == 1) {
            // Get n
            iss >> buf;

            // Get num turns
            iss >> buf;
            temp.turns = std::stoi(buf);

        } else if (line_num == 2) {

            // Load the values into the priority queue
            while (iss >> buf)
                temp.pQueue.push(std::stoi(buf));


        } else if (line_num == 3) {

            // Get the coin toss value
            iss >> buf;

            temp.isTails = buf == "TAILS";

            // Stop the clock and add the push to queue time to the test case
            auto endClock = std::chrono::high_resolution_clock::now();
            double microseconds = std::chrono::duration_cast<std::chrono::microseconds> (endClock - startClock).count();

            temp.microseconds = microseconds;

            // Last line of current test case, add it to the testCases vector and set line num back to 0
            testCases.push_back(temp);
            line_num = 0;

            // Reset temp test case
            temp = TestCase();
        }

        line_num++;
    }
    infile.close();

    return testCases;
}

// Solves a test case
void max_score_solver(TestCase &testCase)
{
    long scott_score = 0;
    long rusty_score = 0;

    // Number of turns for each player
    int max_turns = testCase.turns;

    // While queue still has values
    while (testCase.pQueue.size() > 0)
    {
        // Check who's turn it is
        if (testCase.isTails) {
            // Rusty's turn
            // Run for max_turns time
            int turn = 0;
            while (turn < max_turns && !testCase.pQueue.empty()) {
                // Take a value off the queue for rusty
                rusty_score += testCase.pQueue.top_max_sum();
                testCase.pQueue.pop_max_sum();

                // Next turn
                turn++;
            }

        } else {

            // Scott's turn
            // Run for max_turns time
            int turn = 0;
            while (turn < max_turns && !testCase.pQueue.empty()) {
                // Take a value off the queue for scott
                scott_score += testCase.pQueue.top_max_value();
                testCase.pQueue.pop_max_value();

                // Next turn
                turn++;
            }
        }
        // Change player
        testCase.isTails = !testCase.isTails;
    }

    // Print the results
    std::cout << scott_score << " " << rusty_score << std::endl;

}

int main(int argc, char *argv[]) {

    // If no filename is given, prompt the user and exit program
    if (argv[1] == nullptr) {
        std::cerr << "Error: This Program requires a filename argument." << std::endl;
        return 0;
    }

    std::string filename(argv[1]);

    // Run the algorithm for each Test Case
    for (auto testCase : get_file_data(filename)) {
        // Track algorithm run time
        auto startClock = std::chrono::high_resolution_clock::now();

        // Run solver
        max_score_solver(testCase);

        auto endClock = std::chrono::high_resolution_clock::now();
        double microseconds = std::chrono::duration_cast<std::chrono::microseconds> (endClock - startClock).count();

        // Log time is the solver time plus the original push time
        std::cout << "Solver runtime: " << std::fixed << (testCase.microseconds + microseconds)/1000000<< " seconds\n"  << std::endl;
    }

    return 0;
}
