#include <iostream>
#include <set>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <functional>
#include <tuple>

// -------------- FUNCTOR CLASSES --------

// Hash functor used to store a pair of name and subject as the key
typedef std::pair<std::string, std::string> key_p;
struct pair_hash : public std::unary_function<key_p, std::size_t>
{
    std::size_t operator ()(key_p const &k) const
    {
        return std::hash<std::string>()(k.first) ^ std::hash<std::string>()(k.second);
    }
};

// Tuple to store (name, subject, grade)
typedef std::tuple<std::string, std::string, uint32_t> student_d;

// Unordered map with key of key_p and value of student_d
typedef std::unordered_map<key_p, student_d, pair_hash> map_p;

// Comparator function that sorts a set in name order given a tuple pointer of (name, subject, grade)
class CmpName {
public:
    bool operator()(student_d *aPtr, student_d *bPtr) const
    {
        // Order by chosen fir tuple value, if name equal order by grade, if grade equal order by subject
        const std::string &aName = std::get<0>(*aPtr);
        const std::string &bName = std::get<0>(*bPtr);

        // Compare grade, if grade equal, compare name, if name equal, compare subject
        if (aName < bName)
            return true;
        else if (aName == bName)
            if (std::get<2>(*aPtr) < std::get<2>(*bPtr))
                return true;
            else if (std::get<2>(*aPtr) == std::get<2>(*bPtr))
                return std::get<1>(*aPtr) < std::get<1>(*bPtr);
            else
                return false;
        else
            return false;
    }
};

// Set Type for storing pointers of student_d ordered by name
typedef std::set<student_d*, CmpName> set_nPtr;

// Comparator function that sorts a set in grade order given a tuple pointer of (name, subject, grade)
class CmpGrade {
public:
    bool operator()(student_d *aPtr, student_d *bPtr) const
    {
        // Order by chosen fir tuple value, if name equal order by grade, if grade equal order by subject
        const uint32_t &aGrade = std::get<2>(*aPtr);
        const uint32_t &bGrade = std::get<2>(*bPtr);

        // Compare grade, if grade equal, compare name, if name equal, compare subject
        if (aGrade > bGrade)
            return true;
        else if (aGrade == bGrade)
            if (std::get<0>(*aPtr) < std::get<0>(*bPtr))
                return true;
            else if (std::get<0>(*aPtr) == std::get<0>(*bPtr))
                return std::get<1>(*aPtr) < std::get<1>(*bPtr);
            else
                return false;
        else
            return false;
    }
};

// Set Type for storing pointers of student_d ordered by grade
typedef std::set<student_d*, CmpGrade> set_gPtr;

// -------------- PROGRAM FUNCTIONS -------------

// Populate structures
void populateStructs(map_p &studentRecord, set_nPtr &nameOrder, set_gPtr &gradeOrder)
{
    // Get Student data
    std::ifstream file("indata.txt", std::ios::in);

    std::string name, subject;
    uint32_t grade;
    while (file >> name >> subject >> grade)
    {
        // Add value to hashmap
        studentRecord[key_p(name, subject)] = student_d(name, subject, grade);

        // Populate sets with pointers to student_d tuples
        nameOrder.emplace(&(studentRecord.find(key_p(name, subject))->second));
        gradeOrder.emplace(&(studentRecord.find(key_p(name, subject))->second));

        if (file.eof())
            break;
    }
    file.close();
}

// Prints in orders specified by option, grouped by subject
void printOrderBySubject(uint32_t option, set_nPtr &nameOrder, set_gPtr &gradeOrder, std::string &subject)
{
    if (option == 1) {
        for (auto student : nameOrder)
            if (std::get<1>(*student) == subject)
                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
    } else if (option == 2)
        for (auto student : gradeOrder)
            if (std::get<1>(*student) == subject)
                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
}


// Main menu loop
void programLoop(map_p &studentRecord, set_nPtr &nameOrder, set_gPtr &gradeOrder)
{
    // Subjects
    std::vector<std::string> subjects{"Biology", "Chemistry", "Mathematics", "Physics"};

    int32_t option = 1;
    while (option != -1)
    {
        // Option menu
        std::cout << "\nSelect from the following options.\n";
        std::cout << "1. Order by alphabetical name order.\n";
        std::cout << "2. Order by total marks.\n";
        std::cout << "3. Select a subject group, with sub-options\n";
        std::cout << "4. Search for student\n";
        std::cout << "-1 to exit." << std::endl;
        std::cin >> option;

        // Option 1
        if (option == 1) {
            for (auto student : nameOrder)
                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
        }

            // Option 2
        else if (option == 2) {
            for (auto student : gradeOrder)
                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
        }

            // Option 3
        else if (option == 3) {
            std::cout << "Enter the Subject you wish to view (Enter All for all subjects)." << std::endl;
            std::string subOpt; std::cin >> subOpt;
            // Order option
            std::cout << "1. Order by name\n2. Order by grade" << std::endl;
            uint32_t orderOpt; std::cin >> orderOpt;

            if (subOpt == "All" || subOpt == "all") {
                // Loop All Subjects
                for (auto const &subject : subjects) {
                    if (orderOpt == 1)
                        for (auto student : nameOrder) {
                            if (std::get<1>(*student) == subject)
                                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
                        }
                    else if (orderOpt == 2)
                        for (auto student : gradeOrder)
                            if (std::get<1>(*student) == subject)
                                std::cout << std::get<0>(*student) << " " << std::get<1>(*student) << " " << std::get<2>(*student) << std::endl;
                }
            }
            else if (subOpt == subjects[0] || subOpt == subjects[1] || subOpt == subjects[2] || subOpt == subjects[3] ) {
                printOrderBySubject(orderOpt, nameOrder, gradeOrder, subOpt);
            }
            else { std::cout << "Incorrect Subject choice." << std::endl; }
        }

            // Option 4
        else if (option == 4) {
            std::cout << "Input student name:" << std::endl;
            std::string sName; std::cin >> sName;
            // Get students records from studentRecord hashmap O(1)
            bool exists = true;
            for (const std::string &sSubject : subjects) {
                auto s_it = studentRecord.find(key_p(sName, sSubject));
                if (s_it != studentRecord.end())
                    std::cout << "Grade for " << sSubject << ": " << std::get<2>(s_it->second) << std::endl;
                else {
                    exists = false;
                    break;
                }
            }
            if (!exists){
                // Not found
                std::cout << "Student does not exist, would you like to add them (y/n)" << std::endl;
                char add; std::cin >> add;
                if (add == 'y') {
                    for (const std::string &sSubject : subjects) {
                        std::cout << "Input students grade for " << sSubject << ": ";
                        uint32_t sGrade;
                        std::cin >> sGrade;
                        // Add to studentRecord O(1)
                        studentRecord[key_p(sName, sSubject)] = student_d(sName, sSubject, sGrade);
                        // Add pointer of new student to sets O(logN)
                        nameOrder.emplace(&(studentRecord.find(key_p(sName, sSubject))->second));
                        gradeOrder.emplace(&(studentRecord.find(key_p(sName, sSubject))->second));
                    }
                }
            }

        }
        // Invalid option
        else {
            std::cout << "Incorrect option choice, try again." << std::endl;
        }

    }
}

int main()
{

    // Hashmap of all data O(1) lookup for StudentName Subject
    map_p studentRecord;

    // Set containing pointers to all student_d tuples in the hashmap, in the order of name
    set_nPtr nameOrder;

    // Set containing pointers to all student_d tuples in the hashmap, in the order of grade
    set_gPtr gradeOrder;

    // Populate Structures
    populateStructs(studentRecord, nameOrder, gradeOrder);

    // Run application
    programLoop(studentRecord, nameOrder, gradeOrder);


    return 0;
}
