//
// Created by caleb on 22/3/20.
//

#include "PriorityQueue.h"

int PriorityQueue::parent(const int &i)
{
    return (i-2)/2;
}

int PriorityQueue::child_right(const int &i)
{
    return (2*i+2);
}

int PriorityQueue::child_left(const int &i)
{
    return (2*i+1);
}

void PriorityQueue::bubble_down(const int &i)
{
    // Get the indexes of child nodes of the node at index i
    int r_idx = child_right(i);
    int l_idx = child_left(i);

    // Max is the greatest value out of the current node and its child nodes
    int max_idx = i;

    // Compare the node at i with its children
    if (nodes[l_idx].second > nodes[i].second && l_idx < nodes.size())
        max_idx = l_idx;

    if (nodes[r_idx].second > nodes[max_idx].second && r_idx < nodes.size())
        max_idx = r_idx;

    // If the the node a index i is not larger than its child, swap and recursively run down the tree
    if (max_idx != i) {
        std::swap(nodes[i], nodes[max_idx]);
        bubble_down(max_idx);
    }
}

void PriorityQueue::bubble_up(const int &i)
{
    // Index of the nodes parent
    int p_idx = parent(i);

    // Check if the node at index i has a parent that less than it
    if (nodes[p_idx].second < nodes[i].second && p_idx >= 0) {
        // Swap the parent and child
        std::swap(nodes[i], nodes[p_idx]);

        // Recursively call until the parent is greater than the current child
        bubble_up(p_idx);
    }
}


bool PriorityQueue::empty() { return nodes.empty(); }

uint PriorityQueue::size() { return nodes.size(); }

std::pair<std::string, int> PriorityQueue::top()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        throw std::out_of_range("ERROR: Priority Queue is empty, no value returned.");
    }

    // Return the node with the highest priority
    return nodes[0];
}

void PriorityQueue::pop()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        std::cout << "ERROR: Priority Queue is empty, no value popped.\n" << std::endl;
    } else {
        // Replace the root node with the last leaf node
        nodes[0] = nodes.back();
        nodes.pop_back();

        // Bubble the new root node down to its correct position
        bubble_down(0);
    }
}

void PriorityQueue::push(const std::pair<std::string, int> &node)
{
    // Push the new node to the end of the queue and bubble it up to its position
    nodes.push_back(node);

    bubble_up((int)nodes.size()-1);
}

