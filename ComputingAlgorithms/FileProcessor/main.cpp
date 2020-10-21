#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "File.h"

int main(int argc, char **argv) {

    if (argc < 3) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }

    File fileObj(argv[1], argv[2]);


    fileObj.printFileInfo();

    fileObj.formatTextToFile();

    fileObj.closeFiles();

    return 0;
}
