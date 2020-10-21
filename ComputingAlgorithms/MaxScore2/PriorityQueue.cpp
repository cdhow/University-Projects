//
// Created by caleb on 2/4/20.
//


#include "PriorityQueue.h"


int PriorityQueue::parent(const int &i)
{
    return (i-1)/2;
}

int PriorityQueue::child_right(const int &i)
{
    return (2*i+2);
}

int PriorityQueue::child_left(const int &i)
{
    return (2*i+1);
}

void PriorityQueue::heapify_down_mNodes(const int &i)
{
    // Get the indexes of child nodes of the node at index i
    int r_idx = child_right(i);
    int l_idx = child_left(i);

    // Max index is the greatest value out of the current node at i and its child nodes
    int max_idx = i;

    // Compare the node at i with its children ( .first is the value not the sum of digits)
    if (l_idx < mNodes.size() && mNodes[l_idx].first > mNodes[i].first)
        max_idx = l_idx;

    if (r_idx < mNodes.size() && mNodes[r_idx].first > mNodes[max_idx].first)
        max_idx = r_idx;

    // If the the node a index i is not larger than its child, swap and recursively run down the tree
    if (max_idx != i) {
        std::swap(mNodes[i], mNodes[max_idx]);
        heapify_down_mNodes(max_idx);
    }
}

void PriorityQueue::heapify_down_mSumNodes(const int &i)
{
    // Get the indexes of child nodes of the node at index i
    int r_idx = child_right(i);
    int l_idx = child_left(i);

    // Max index is the greatest value out of the current node at i and its child nodes
    int max_idx = i;

    // Compare the node at i with its children ( .second is the sum of digits not the value)
    // If the sums of digits are equal, compare the values instead
    if (l_idx < mSumNodes.size()
        && (mSumNodes[l_idx].second > mSumNodes[i].second
        || (mSumNodes[l_idx].second == mSumNodes[i].second
        && mSumNodes[l_idx].first > mSumNodes[i].first)))
        { max_idx = l_idx; }

    if (r_idx < mSumNodes.size()
        && (mSumNodes[r_idx].second > mSumNodes[max_idx].second
        || (mSumNodes[r_idx].second == mSumNodes[max_idx].second
        && mSumNodes[r_idx].first > mSumNodes[max_idx].first)))
        { max_idx = r_idx; }

    // If the the node at index i is not larger than its children, swap and recursively run down the tree
    if (max_idx != i) {
        std::swap(mSumNodes[i], mSumNodes[max_idx]);
        heapify_down_mSumNodes(max_idx);
    }
}

void PriorityQueue::heapify_up_mNodes(const int &i)
{
    // Index of the nodes parent
    int p_idx = parent(i);

    // Check if the node at index i has a parent that less than it
    if (p_idx >= 0 && mNodes[p_idx].first < mNodes[i].first) {
        // Swap the parent and child
        std::swap(mNodes[i], mNodes[p_idx]);

        // Recursively call until the parent is greater than the current child
        heapify_up_mNodes(p_idx);
    }
}

void PriorityQueue::heapify_up_mSumNodes(const int &i)
{
    // Index of the nodes parent
    int p_idx = parent(i);

    // Check if the node at index i has a parent's sum of digits that less than its sum of digits,
    // If the sums of digits are equal, compare the nodes' values instead
    if (p_idx >= 0
        && (mSumNodes[p_idx].second < mSumNodes[i].second
        || (mSumNodes[p_idx].second == mSumNodes[i].second
        && mSumNodes[p_idx].first < mSumNodes[i].first)))
        {
            // Swap the parent and child
            std::swap(mSumNodes[i], mSumNodes[p_idx]);

            // Recursively call until the parent is greater than the current child
            heapify_up_mSumNodes(p_idx);
        }
}

// mNodes and nSumNodes will always be the same size
bool PriorityQueue::empty() { return mNodes.empty(); }

uint PriorityQueue::size() { return mNodes.size(); }

int PriorityQueue::top_max_value()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        throw std::out_of_range("ERROR: Priority Queue is empty, no value returned.");
    }

    // Return the node with the highest priority
    return mNodes[0].first;
}

int PriorityQueue::top_max_sum()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        throw std::out_of_range("ERROR: Priority Queue is empty, no value returned.");
    }

    // Return the node with the highest priority
    return mSumNodes[0].first;
}

void PriorityQueue::pop_max_value()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        std::cout << "ERROR: Priority Queue is empty, no value popped.\n" << std::endl;
    } else {
        // Get the top value from the mNodes heap
        std::pair<int,int> val = mNodes[0];

        // Find the location (index) of val in mSumNodes
        int i = std::find(mSumNodes.begin(), mSumNodes.end(), val) - mSumNodes.begin();

        // Consider i to be the root node of mSumNodes
        // Replace the root node with the last leaf node and heapify down from there
        mSumNodes[i] = mSumNodes.back();
        mSumNodes.pop_back();
        // Bubble the new root node down to its correct position
        heapify_down_mSumNodes(i);

        // Now for the mNodes queue
        // Replace the root node with the last leaf node
        mNodes[0] = mNodes.back();
        mNodes.pop_back();

        // Bubble the new root node down to its correct position
        heapify_down_mNodes(0);
    }
}

void PriorityQueue::pop_max_sum()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        std::cout << "ERROR: Priority Queue is empty, no value popped.\n" << std::endl;
    } else {
        // Get the top value from the mSumNodes heap
        std::pair<int,int> val = mSumNodes[0];

        // Find the location (index) of val in mNodes
        int i = std::find(mNodes.begin(), mNodes.end(), val) - mNodes.begin();

        // Consider i to be the root node of mNodes
        // Replace the root node with the last leaf node and heapify down from there
        mNodes[i] = mNodes.back();
        mNodes.pop_back();
        // Bubble the new root node down to its correct position
        heapify_down_mNodes(i);

        // Now for the mSumNodes queue
        // Replace the root node with the last leaf node
        mSumNodes[0] = mSumNodes.back();
        mSumNodes.pop_back();

        // Bubble the new root node down to its correct position
        heapify_down_mSumNodes(0);
    }
}

void PriorityQueue::push(const int &val)
{
    int temp = val;
    // Calculate sum of digits
    int sum = 0;
    while (temp) {
        sum += temp % 10;
        temp /= 10;
    }

    // Push the new node to the end of each heap and bubble it up to its position

    mNodes.emplace_back(val, sum);
    heapify_up_mNodes((int)mNodes.size()-1);

    mSumNodes.emplace_back(val, sum);
    heapify_up_mSumNodes((int)mSumNodes.size()-1);

}