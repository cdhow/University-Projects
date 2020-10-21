//
// Created by caleb on 12/8/19.
//

#include "List.h"


// -------- Constructors & Assignments ------- //

// No input
List::List() {
    items = new int[capacity];
    // Initialise random seed
    srand(time(nullptr));
}

// Specified capacity
List::List(int cap) : capacity{cap} {
    items = new int[capacity];
    // Initialise random seed
    srand(time(nullptr));
}

// Copy constructor
List::List(List const &that) : capacity{that.capacity}, itemCount{that.itemCount}, sorted{that.sorted} {
    items = new int[capacity];
    // Copy that.items array
    for (int i = 0; i < itemCount; i++)
        items[i] = that.items[i];

}

// Move Constructor
List::List(List && that) noexcept : capacity{std::move(that.capacity)}, itemCount{std::move(that.itemCount)},
                                    sorted{std::move(that.sorted)}, items{std::move(that.items)} {
    that.items = nullptr;
}

// Copy Assignment
List & List::operator =(List const &that) {
    if (this != &that) {
        capacity = that.capacity, itemCount = that.itemCount, sorted = that.sorted;
        items = new int[capacity];

        // Copy that.items array
        for (int i = 0; i < itemCount; i++)
            items[i] = that.items[i];
    }
    return *this;
}

// Move Assignment
List & List::operator =(List && that) noexcept {
    capacity = that.capacity, itemCount = that.itemCount, sorted = that.sorted;
    items = that.items;
    that.items = nullptr;
}

// Destructor
List::~List() {
    delete[] items;
}


// ------------ OTHER METHODS ------------ //

// Use privately to update items array size at the current capacity
void List::updateItemsSize() {
    int *tempValues = new int[capacity];
    // Copy values into temp array
    for (int i=0; i < itemCount; i++)
        tempValues[i] = items[i];

    // Assign temp array to the items array
    delete[] items;
    items = tempValues;
    delete[] tempValues;
}

int List::getCapacity() {
    return capacity;
}

int List::getItemCount() {
    return itemCount;
}

int List::operator[](int index) {
    if (index >= itemCount)
        std::cerr << "Indexed value exceeds list range. Value is invalid." << std::endl;
    return items[index];
}

// Sort items array in increasing order
void List::sortItems() {
    std::sort(items, items + itemCount);
    sorted = true;
}

// Unsort (scramble items)
void List::unsortItems() {
    // Loop through list
    for (int i = 0; i < itemCount; i++) {
        // Get random index that != i
        int idx = i;
        while (idx == i)
            idx = rand() % itemCount;
        // Swap items in array
        int temp = items[i];
        items[i] = items[idx];
        items[idx] = temp;
    }
    sorted = false;
}

bool List::sortStatus() {
    return sorted;
}

// Extend array by doubling capacity
void List::extend() {
    capacity *= 2;
    // Change items array Size
    updateItemsSize();
}

// Shrink array by halving
void List::shrink() {
    // Only execute shrink if itemCount <= capacity/2
    if (itemCount > capacity/2) {
        std::cerr << "Cannot shrink as list item count exceeds shrink size." << std::endl;
        std::cerr << "Capacity = " << capacity << ". Cannot shrink to " << capacity/2 << std::endl;
    } else {
        capacity /= 2;
        // Change items array Size
        updateItemsSize();
    }
}

// Insert values into items array
void List::insert(int value) {
    // Increase capacity by 1 if capacity is full
    if (capacity == itemCount) {
        std::cout << "List Capacity full, incrementing capacity by 1..." << std::endl;
        capacity += 1;
        updateItemsSize();
    }

    if (!sorted) {
        // Add value to end of items array
        items[itemCount] = value;
    } else if (sorted) {
        // Loop through items backwards, shifting each item in array
        // until the value is in correct position
        for (int i = (itemCount - 1); i <= 0; i--) {
            if (items[i] > value) {
                // Shift value
                items[i+1] = items[i];
                items[i] = 0;
            } else {
                // Value is less then, therefore insert a i+1
                items[i+1] = value;
                break;
            }
        }
    }
    // Increment itemCount
    itemCount += 1;
}

// Binary Search used for a sorted search
int List::binarySearch(int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        if (items[mid] == x)
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (items[mid] > x)
            return binarySearch(l, mid - 1, x);

        // Else the element can only be present
        // in right subarray
        return binarySearch(mid + 1, r, x);
    }
    // We reach here when element is not
    // present in array
    return -1;
}

int List::search(int value) {
    if (!sorted) {
        // Check the entire array for the first occurrence of the value
        for (int i = 0; i < itemCount; i++) {
            if (items[i] == value)
                return i;
        }
    } else if (sorted) {
        // Use a binary search to locate value
        int idx = binarySearch(0, itemCount, value);
        if (idx >= 0)
            return idx;
    }
    // Value not found, return capacity
    return capacity;
}

bool List::remove(int value) {
    // Get value index
    int idx = search(value);
    if (idx == capacity) {
        // Value not found
        return false;
    } else {
        if (!sorted) {
            // Change value to last item
            items[idx] = items[itemCount-1];
            items[itemCount-1] = 0;
        } else if (sorted) {
            // If idx is the last value
            if (idx == itemCount - 1) {
                items[idx] = 0;
            } else {
                // Start at index value, shift all i to equal i + 1
                for (int i = idx; i < itemCount - 1; i++)
                    items[i] = items[i + 1];
                items[itemCount-1] = 0;
            }
        }
        return true;
    }
}

void List::display() {
    std::cout << "Item Count: " << itemCount << std::endl;
    for (int i = 0; i < itemCount; i++) {
        std::cout << items[i] << "  ";
        if (i % 10 == 0 && i != 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;

}