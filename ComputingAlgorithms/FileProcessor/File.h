//
// Created by caleb on 18/07/19.
//

#ifndef PROJECT1_FILE_H
#define PROJECT1_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class File {
protected:
    std::ifstream infile;
    std::ofstream outfile;

    // Corresponds to the alphabet holds counts
    int letter_occurrence[26] = {0};

    // Used to rewind file stream
    void rewind_stream();

    // Update letter_occurrences
    void update_loccurs();


public:
    File(std::string&&, std::string&&);

    int lineCount();

    int wordCount();

    std::string largestWord();

    void printFileInfo();

    void formatTextToFile();

    void closeFiles();

};


#endif //PROJECT1_FILE_H
