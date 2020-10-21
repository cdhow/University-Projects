//
// Created by caleb on 22/3/20.
//

#ifndef MAXSCORE_PRIORITYQUEUE_H
#define MAXSCORE_PRIORITYQUEUE_H


#include <vector>
#include <stdexcept>
#include <iostream>


class PriorityQueue {
private:
    // Vector to store nodes, each node is a pair that contains a value and its original index
    // the original index will be used in the program to access a boolean value from a hash map
    // with key type "value-index" and value canBeUsed to tell us whether a value and be used
    std::vector<std::pair<std::string, int>> nodes;

    // Returns the index of the the parent node of nodes[i]
    int parent(const int &);

    // Returns the index of the right child node of nodes[i]
    int child_right(const int &i);

    // Returns the index of the left child node of nodes[i]
    int child_left(const int &);

    // This method is used for the pop() method to bubble the new root node down to its correct position
    void bubble_down(const int &);

    // This method is used for the push() method to bubble the newly pushed node up to its correct
    void bubble_up(const int &);

public:

    // Returns true if priority queue is empty
    bool empty();

    // Returns the size of the priority queue
    uint size();

    // Returns the root node
    std::pair<std::string, int> top();

    // Pops the root node
    void pop();

    // Pushes a new node to the priority queue
    void push(const std::pair<std::string, int> &);

};


#endif //MAXSCORE_PRIORITYQUEUE_H
