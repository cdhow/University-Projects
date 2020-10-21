//
// Created by caleb on 18/07/19.
//

#include "File.h"

    // Used to rewind file stream
    void File::rewind_stream() {
        infile.clear();
        infile.seekg(0, std::ios::beg);
    }

    // Update letter_occurrences
    void File::update_loccurs() {
        char alphabet[] = "abcdefghijklmnopqrztuvwxyz";

        std::string word;
        // Loop file, grab a word
        while (infile >> word) {
            // Loop word, grab a char
            for (int i = 0; i < word.length(); i++) {
                // Compare current char against alphabet
                for (int j = 0; j < 26; j++) {
                    // word[i] or word[i] + 32 if uppercase
                    if (word[i] == alphabet[j] || (word[i] + 32) == alphabet[j])
                        letter_occurrence[j]++;
                }
            }
        }
        rewind_stream();
    }



    File::File(std::string&& infileName, std::string&& outfileName) {
        infile.open(infileName);
        outfile.open(outfileName);

        update_loccurs();
    }

    int File::lineCount() {
        int count{0};
        std::string line;
        while(std::getline(infile, line))
            count++;

        rewind_stream();

        return count;
    }

    int File::wordCount() {
        int count{0};
        std::string word;
        while (infile >> word)
            count++;

        rewind_stream();

        return count;
    }

    std::string File::largestWord() {
        std::string word;
        std::string largeWord;
        while (infile >> word)
            if (word.length() > largeWord.length())
                largeWord = word;

        rewind_stream();

        return largeWord;
    }

    void File::printFileInfo() {
        outfile << "######## Information About the Input File ########\n" << std::endl;
        outfile << "Line count: " << lineCount() << std::endl;
        outfile << "Word count: " << wordCount() << std::endl;
        outfile << "Largest word: " << largestWord() << std::endl;
        outfile << std::endl;

    }

    void File::formatTextToFile() {
        outfile << "######## Formatted Text From Input File ########\n" << std::endl;

        std::string line;
        std::string word;
        // Get line
        while(std::getline(infile, line)) {
            // Skip if empty line
            if (line.length() >= 1) {

                // Create stream from line
                std::istringstream iss(line);
                // Loop through words
                while (iss >> word) {
                    // First letter to uppercase
                    if (islower(word[0]))
                        word[0] -= 32;
                    // All other letters to lowercase
                    for (int i = 1; i < word.length(); i++) {
                        if (isupper(word[i]))
                            word[i] += 32;
                    }
                    // Print to file followed by space
                    outfile << word << " ";
                }
                // Newline
                outfile << std::endl;
            }
        }

        rewind_stream();

    }

    void File::closeFiles() {
        infile.close();
        outfile.close();
    }
