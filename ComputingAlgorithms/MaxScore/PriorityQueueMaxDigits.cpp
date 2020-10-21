//
// Created by caleb on 22/3/20.
//

#include "PriorityQueueMaxDigits.h"

int PriorityQueueMaxDigits::parent(const int &i)
{
    return (i-2)/2;
}

int PriorityQueueMaxDigits::child_right(const int &i)
{
    return (2*i+2);
}

int PriorityQueueMaxDigits::child_left(const int &i)
{
    return (2*i+1);
}

void PriorityQueueMaxDigits::bubble_down(const int &i)
{
    // Get the indexes of child nodes of the node at index i
    int r_idx = child_right(i);
    int l_idx = child_left(i);

    // Max is the greatest value out of the current node and its child nodes
    int max_idx = i;

    // Compare the node at i with its children
    if (l_idx < nodes.size() && summations[nodes[l_idx].first] > summations[nodes[i].first])
        max_idx = l_idx;

    if (r_idx < nodes.size() && summations[nodes[r_idx].first] > summations[nodes[max_idx].first])
        max_idx = r_idx;

    // If the the node a index i is not larger than its child, swap and recursively run down the tree
    if (max_idx != i) {
        std::swap(nodes[i], nodes[max_idx]);
        bubble_down(max_idx);
    }
}

void PriorityQueueMaxDigits::bubble_up(const int &i)
{
    // Index of the nodes parent
    int p_idx = parent(i);

    // Check if the summation of digits of the node at index i has a parent that's summation is less than it
    if (p_idx >= 0 && summations[nodes[p_idx].first] < summations[nodes[i].first]) {
        // Swap the parent and child
        std::swap(nodes[i], nodes[p_idx]);

        // Recursively call until the parent is greater than the current child
        bubble_up(p_idx);
    }
}


bool PriorityQueueMaxDigits::empty() { return nodes.empty(); }

uint PriorityQueueMaxDigits::size() { return nodes.size(); }

std::pair<std::string, int> PriorityQueueMaxDigits::top()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        throw std::out_of_range("ERROR: Priority Queue is empty, no value returned.");
    }

    // Return the node with the highest priority
    return nodes[0];
}

void PriorityQueueMaxDigits::pop()
{
    // Check that the priority queue has nodes in it
    if (empty()) {
        std::cout << "ERROR: Priority Queue is empty, no value popped.\n" << std::endl;
    } else {
        // Remove the summation to save space
        summations.erase(nodes[0].first);

        // Replace the root node with the last leaf node
        nodes[0] = nodes.back();
        nodes.pop_back();

        // Bubble the new root node down to its correct position
        bubble_down(0);
    }
}

void PriorityQueueMaxDigits::push(const std::pair<std::string, int> &node)
{
    // Calculate the summation of the value
    int value = node.second;

    int sum = 0;
    while (value > 0) {
        sum += value % 10;
        value /= 10;
    }

    // Add it to the map of summations
    summations[node.first] = sum;

    // Push the new node to the end of the queue and bubble it up to its position
    nodes.push_back(node);

    bubble_up((int)nodes.size()-1);
}
