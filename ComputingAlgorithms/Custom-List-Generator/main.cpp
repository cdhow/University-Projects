#include <iostream>
#include "List.h"
int main() {
    // Starter List
    List list;
    list.insert(12);
    list.insert(8);
    list.insert(9);

    std::cout << "Please Choose from the following options:\n";
    std::cout << "1 Get capacity\n2 Get item count\n3 Get item at index\n";
    std::cout << "4 Sort items\n5 Unsort items\n6 Get sort status\n";
    std::cout << "7 Double capacity\n8 Halve Capacity\n9 Insert element\n";
    std::cout << "10 Get index at item (search)\n11 Remove item\n12 Display list info\n-1 Quit loop" << std::endl;

    int option = 100;
    while (option != -1) {
        std::cin >> option;
        switch (option) {
            case 1 : std::cout << list.getCapacity() << std::endl;
                break;
            case 2 : std::cout << list.getItemCount() << std::endl;
                break;
            case 3 :
                int opt;
                std::cout << "Choose an index: \n"; std::cin >> opt;
                std::cout << list[opt] << std::endl;
                break;
            case 4 : list.sortItems();
                break;
            case 5 : list.unsortItems();
                break;
            case 6 : std::cout << list.sortStatus() << std::endl;
                break;
            case 7 : list.extend();
                break;
            case 8 : list.shrink();
                break;
            case 9 :
                int val;
                std::cout << "Input value to insert: \n"; std::cin >> val;
                list.insert(val);
                break;
            case 10 :
                int item;
                std::cout << "What items index were you after?: \n"; std::cin >> item;
                std::cout << list.search(item) << std::endl;
                break;
            case 11 :
                int rev;
                std::cout << "What item do you want to remove?: \n"; std::cin >> rev;
                std::cout << list.remove(rev) << std::endl;
                break;
            case 12 : list.display();
                break;
        }
        if (option != -1)
            std::cout << "Choose again:" << std::endl;
    }

    return 0;
}