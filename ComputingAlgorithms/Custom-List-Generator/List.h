//
// Created by caleb on 12/8/19.
//

#ifndef ASS2TASK1_LIST_H
#define ASS2TASK1_LIST_H

#include <iostream>
#include <algorithm>
#include <random>

class List {
private:
    int capacity{8};
    int itemCount{0};
    int *items;
    bool sorted = false;

    // Update items array capacity
    void updateItemsSize();

    // A recursive binary search function. It returns
    // location of x in given array arr[l..r] is present,
    // otherwise -1
    int binarySearch(int, int, int);

public:
    // Default constructors
    List();
    List(int);

    // Copy constructor
    List(List const &);

    // Move constructor
    List(List &&) noexcept;

    // Copy assignment
    List & operator =(List const &);

    // Move assignment
    List & operator =(List && that) noexcept;

    // Destructor
    ~List();

    // Get capacity
    int getCapacity();

    // Get item count
    int getItemCount();

    // Get item at index
    int operator[](int index);

    // Sort items
    void sortItems();

    // Unsort items
    void unsortItems();

    bool sortStatus();

    // Extend array by doubling capacity
    void extend();

    // Shrink array by halving
    void shrink();

    // Inserts integer at end if not sorted or
    // at sorted position if array is sorted
    void insert(int);

    // Searches for a value in items and returns its index
    int search(int);

    // Removes an item from items array, returns false if item is not in array
    bool remove(int);

    // Displays itemCount and all items in list
    void display();


};

#endif //ASS2TASK1_LIST_H
