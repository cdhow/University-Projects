#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include "PriorityQueue.h"
#include "PriorityQueueMaxDigits.h"

/*
 * Algorithm
 * Get number of test cases
 * Get
 */

// This class contains the data for a single test case that has been loaded in from the input file
class TestCase {
public:
    // Number of ball on the table
    int n;

    // Maximum number of turns allowed per round
    int num_turns;

    // Max Heap priority queue
    PriorityQueue max_queue;

    // Max summations queue
    PriorityQueueMaxDigits max_digits_queue;

    // Coin toss value, (Rusty's turn is tails)
    bool is_tails;

    // Because there are two priority queues, one for Scott and one for Rusty, these maps uses the key "value-index"
    // to return a boolean value that tells if the value has been chosen already
    std::unordered_map<std::string, bool> chosen;

};

// TODO have a hashmap that allows up to check for values that have already been used
//  use a hash map of (key="val-index", value= bool hasBeenChosen"
//  this mean we will have to have the priorty queues store pairs of value,original index

// Takes an input filename and returns a vector of TestCases
std::vector<TestCase> get_file_data(const std::string &filename)
{
    std::vector<TestCase> test_cases;

    std::ifstream infile;
    infile.open(filename);

    // A line
    std::string str;

    // First line is the number of test cases
    std::getline(infile, str);

    test_cases.reserve(std::stoi(str));

    // Temp testCase
    TestCase temp = TestCase();

    // Loop line by line
    int line_num = 1;
    while (std::getline(infile, str))
    {
        // Create a string stream from the current line
        std::string buf; // buffer string
        std::istringstream iss(str);

        if (line_num == 1) {
            // Get n
            iss >> buf;
            temp.n = std::stoi(buf);

            // Get num turns
            iss >> buf;
            temp.num_turns = std::stoi(buf);

        } else if (line_num == 2) {

            // Load the values into the priority queues
            int idx = 0; // index of the value being loaded in
            while (iss >> buf) {
                int value = std::stoi(buf);
                std::string key = buf + "-" + std::to_string(idx);

                // Push to Queues
                temp.max_queue.push(std::make_pair(key, value));
                temp.max_digits_queue.push(std::make_pair(key, value));

                // Push to chosen
                temp.chosen[key] = false;
                idx++;
            }

        } else if (line_num == 3) {

            // Get the coin toss value
            iss >> buf;

            temp.is_tails = buf == "TAILS";

            // Last line of current test case, add it to the test_cases vector and set line num back to 0
            test_cases.push_back(temp);
            line_num = 0;

            // Reset temp test case
            temp = TestCase();
        }

        line_num++;
    }
    infile.close();

    return test_cases;
}

// Solves a test case
void max_score_solver(TestCase &testCase)
{
    long scott_score = 0;
    long rusty_score = 0;

    // Number of turns for each player
    int max_turns = testCase.num_turns;

    // While either queues still have values in them
    while (testCase.max_digits_queue.size() > 0 || testCase.max_queue.size() > 0)
    {
        // Check who's turn it is
        if (testCase.is_tails)
        {
            // Rusty's turn

            // Run for max_turns time
            int turn = 0;
            while (turn < max_turns) {
                // Take a value off rusty's queue
                std::pair<std::string, int> val = testCase.max_digits_queue.top();
                testCase.max_digits_queue.pop();

                // Check that the value has not already been chosen by Scott
                if (!testCase.chosen[val.first]) {
                    rusty_score += val.second;

                    // Set the value to chosen
                    testCase.chosen[val.first] = true;

                    // Include turn
                    turn++;
                }

                // If we run out of values
                if (testCase.max_digits_queue.size() == 0)
                    break;
            }
        }
        else
        {
            // Scott's turn

            // Run for max_turns time
            int turn = 0;
            while (turn < max_turns) {
                // Take a value off scott's queue
                std::pair<std::string, int> val = testCase.max_queue.top();
                testCase.max_queue.pop();

                // Check that the value has not already been chosen by Rusty
                if (!testCase.chosen[val.first]) {
                    scott_score += val.second;

                    // Set the value to chosen
                    testCase.chosen[val.first] = true;

                    // Include turn
                    turn++;
                }

                // If we run out of values
                if (testCase.max_queue.size() == 0)
                    break;
            }
        }
        // Change turns
        testCase.is_tails = !testCase.is_tails;
    }

    // Print the results
    std::cout << scott_score << " " << rusty_score << std::endl;

}

int main() {

    std::vector<TestCase> cases = get_file_data("input.txt");

    // Run the algorithm for each Test Case
    for (auto testCase : cases) {
        // Track algorithm run time
        auto startClock = std::chrono::high_resolution_clock::now();

        // Run solver
        max_score_solver(testCase);

        auto endClock = std::chrono::high_resolution_clock::now();
        double microseconds = std::chrono::duration_cast<std::chrono::microseconds> (endClock - startClock).count();

        // Log time
        std::cout << "Solver runtime: " << microseconds/1000000 << " seconds\n"  << std::endl;
    }


    return 0;
}
