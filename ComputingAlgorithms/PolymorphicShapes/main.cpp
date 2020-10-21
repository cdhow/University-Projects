#include <iostream>
#include "Screen.h"
#include "Shapes/Shape.h"
#include "Shapes/Point.h"
#include "Shapes/Polygon.h"
#include "Shapes/Line.h"
#include "Shapes/Ellipse.h"
#include "Bunch.h"
#include "fstream"

// ---------------- Data Handling --------------
void store_data_to_file(Bunch<Shape *> & bunch, const std::string & filename)
{
    // Open file
    std::ofstream outfile;
    outfile.open("shape_data/" + filename, std::ios::app);
    // Append Files data
    for (int i=0; i < bunch.get_itemCount(); i++)
        outfile << bunch[i]->info();;
    outfile.close();
}

void load_file_data(Bunch<Shape *> & shapes, const std::string & filename)
{
    // Open file
    std::ifstream infile;
    infile.open("shape_data/" + filename);
    if (!infile.is_open()) {
        std::cout << "File does not exist, loading data failed!" << std::endl;
        return;
    }
    // Clear bunch
    shapes.clear_bunch();
    // Loop infile
    while (infile)
    {
        std::string line;
        // Get Shape type in string format
        getline(infile, line, ' ');
        if (!infile)
            break;
        // If it is a Point
        if (line == "Point") {
            // Get variables
            int x, y; char s;
            infile >> x >> y >> s;
            shapes.push(new Point(x, y, s));
        } else {
            // Get variables
            int x, y, xp, yp; char s;
            infile >> x >> y >> xp >> yp >> s;
            // If it is a Line
            if (line == "Line") {
                shapes.push(new Line(x, y, xp, yp, s));
            } else
                // If it is a Ellipse
            if (line == "Ellipse") {
                shapes.push(new Ellipse(x, y, xp, yp, s));
            } else
                // If it is a Polygon
            if (line == "Polygon") {
                shapes.push(new Polygon(x, y, xp, yp, s));
            }
        }
        // Discard newline characters
        infile >> std::ws;
    }
    infile.close();
}

// --------------- Displaying shapes ------------- //
void display_shapes(Bunch<Shape *> & shapes, int idx, Screen & screen)
{
    if (idx >=0 && idx <= shapes.get_itemCount()) {
        // Draw all shapes
        if (idx == shapes.get_itemCount()) {
            for (int i = 0; i < shapes.get_itemCount(); i++)
                shapes[i]->draw(screen);
        } else {
            // Draw chosen shape
            shapes[idx]->draw(screen);
        }
        screen.display();
    } else {
        std::cerr << "Incorrect index value" << std::endl;
    }
    screen.clear();
}

// --------------- Main -------------------
int main()
{
    Bunch<Shape *> shapes;
    Screen screen;

    // MENU
    // Opt 1 creating obj // select object type // add to bunch
    // Opt 2 List items
    // Opt 3 create new screen and display chosen obj // get obj bunch type // get obj index
    // if index input == itemCount, draw all objects to same screen
    int option = 0;
    while (option != -1)
    {
        std::cout << "Please choose from the following options:\n";
        std::cout << "1. Create a new shape\n";
        std::cout << "2. List your shapes\n";
        std::cout << "3. Display a chosen shape\n";
        std::cout << "4. Save or Load shapes from file.\n";
        std::cout << "-1. Exit" << std::endl;
        std::cin >> option;
        // Main Switch Options
        switch (option)
        {
            // Choose a shape and add to a bunch
            case 1 :
                std::cout << "Please choose from the following:\n";
                std::cout << "1. Point\n2. Ellipse\n3. Polygon\n4. Line" << std::endl;
                int opt_1; std::cin >> opt_1;
                std::cout << "Please input the associated variables in format x y s." << std::endl;
                int x, y, a, b;
                char s;
                std::cin >> x >> y >> s;
                switch (opt_1) {
                    case 1 :
                        shapes.push(new Point(x, y, s));
                        break;
                    case 2 :
                        std::cout << "Input a b." << std::endl;
                        std::cin >> a >> b;
                        shapes.push(new Ellipse(x, y, a, b, s));
                        break;
                    case 3 :
                        std::cout << "Input n l." << std::endl;
                        std::cin >> a >> b;
                        shapes.push(new Polygon(x, y, a, b, s));
                        break;
                    case 4 :
                        std::cout << "Input xp yp." << std::endl;
                        std::cin >> a >> b;
                        shapes.push(new Line(x, y, a, b, s));
                    default: std::cout << "Invalid shape choice" <<std::endl;
                }
                break;
            // List Items
            case 2 :
                for (int i=0; i<shapes.get_itemCount(); i++)
                    std::cout << shapes[i]->info();
                std::cout << std::endl;
                break;
            // Display chosen shape
            case 3 :
                int idx;
                std::cout << "Input shapes index in bunch" << std::endl;
                std::cin >> idx;
                display_shapes(shapes, idx, screen);
                screen.display();
                screen.clear();
                break;
            // Save / Load data
            case 4 :
                std::cout << "Choose the file: " << std::endl;
                std::string fname; std::cin >> fname;
                std::cout << "Choose 1 for save or 2 for load: ";
                int opt_3; std::cin >> opt_3;
                if (opt_3 == 1) {
                    store_data_to_file(shapes, fname);
                } else {
                    load_file_data(shapes, fname);
                }
                break;
//            default: if(option != -1) std::cout << "Not an option choose again\n" << std::endl;
        }
    }

}