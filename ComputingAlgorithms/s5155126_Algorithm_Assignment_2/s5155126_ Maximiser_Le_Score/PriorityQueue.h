//
// Created by caleb on 2/4/20.
//

#ifndef MAXSCORE2_PRIORITYQUEUE_H
#define MAXSCORE2_PRIORITYQUEUE_H


#include <vector>
#include <stdexcept>
#include <iostream>

class PriorityQueue {

    // This vector stores all the (value,sum) pairs in a max heap configuration
    std::vector<std::pair<int,int>> mNodes;

    // This vector stores all the (value,sum) pairs in a max heap configuration based on the sum of each values digits
    std::vector<std::pair<int,int>> mSumNodes;

    // Returns the index of the the parent node of mNodes[i] and mSumNodes[i]
    int parent(const int &);

    // Returns the index of the right child node of mNodes[i] and mSumNodes[i]
    int child_right(const int &i);

    // Returns the index of the left child node of mNodes[i] and mSumNodes[i]
    int child_left(const int &);

    // This method is used for the pop() method to bubble the new root node down to its correct position
    // in mNodes
    void heapify_down_mNodes(const int &);

    // This method is used for the pop() method to bubble the new root node down to its correct position
    // in mSumNodes
    void heapify_down_mSumNodes(const int &);

    // This method is used for the push() method to bubble the newly pushed node up to its correct position
    // in mNodes
    void heapify_up_mNodes(const int &);

    // This method is used for the push() method to bubble the newly pushed node up to its correct position
    // in mSumNodes
    void heapify_up_mSumNodes(const int &);

public:

    // Returns true if priority queue is empty
    bool empty();

    // Returns the size of the priority queue
    uint size();

    // Returns the root node of mNodes
    int top_max_value();

    // Returns the root node of mSumNodes
    int top_max_sum();

    // Pops the root node of mNodes and also pops that value from mSumNodes
    void pop_max_value();

    // Pops the root node of mSumNodes and also pops that value from mNodes
    void pop_max_sum();

    // Pushes a new node to the priority queue
    void push(const int &);

};


#endif //MAXSCORE2_PRIORITYQUEUE_H
