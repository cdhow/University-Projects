#include <iostream>
#include "Screen.h"
#include "Shapes/Polygon.h"
#include "Shapes/Ellipse.h"
#include "Shapes/Line.h"
#include "Bunch.h"
#include "fstream"

// ---------------- Data Handling --------------
template <typename T>
void store_data_to_file(Bunch<T> & bunch, const std::string & filename)
{
    // Open file
    std::ofstream outfile;
    outfile.open("shape_data/" + filename, std::ios::app);
    // Append Files data
    for (int i=0; i < bunch.get_itemCount(); i++)
        outfile << bunch[i].info();;
    outfile.close();
}


template <typename T>
void update_bunch(Bunch<T> & bunch, std::ifstream & is)
{
    // Get variables
    int x, y, xp, yp; char s;
    is >> x >> y >> xp >> yp >> s;
    bunch.push(T(x, y, xp, yp, s));
}

void load_file_data(Bunch<Point> & points,Bunch<Line> & lines, Bunch<Ellipse> & ellipses,
                    Bunch<Polygon> & polys, const std::string & filename)
{
    // Open file
    std::ifstream infile;
    infile.open("shape_data/" + filename);
    if (!infile.is_open()) {
        std::cout << "File does not exist, loading data failed!" << std::endl;
        return;
    }
    // Clear all bunches
    points.clear_bunch();
    lines.clear_bunch();
    ellipses.clear_bunch();
    polys.clear_bunch();

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
            points.push(Point(x, y, s));
        } else
            // If it is a Line
        if (line == "Line") {
            update_bunch(lines, infile);
        } else
            // If it is a Ellipse
        if (line == "Ellipse") {
            update_bunch(ellipses, infile);
        } else
            // If it is a Polygon
        if (line == "Polygon") {
            update_bunch(polys, infile);
        }
        // Discard newline characters
        infile >> std::ws;
    }
    infile.close();
}

// --------------- Displaying shapes ------------- //
template <typename T>
void display_shapes(Bunch<T> & bunch, int idx, Screen & screen)
{
    if (idx >=0 && idx <= bunch.get_itemCount()) {
        if (idx == bunch.get_itemCount()) {
            for (int i = 0; i < bunch.get_itemCount(); i++)
                bunch[i].draw(screen);
        } else {
            bunch[idx].draw(screen);
        }
        screen.display();
    } else {
        std::cerr << "Incorrect index value" << std::endl;
    }
    screen.clear();
}

template <typename T>
void draw_bunch(Bunch<T> & bunch, Screen & screen)
{
    for (int i=0; i<bunch.get_itemCount(); i++)
        bunch[i].draw(screen);
}

// --------------- Main -------------------
int main()
{
    Bunch<Point> points;
    Bunch<Ellipse> ellipses;
    Bunch<Polygon> polys;
    Bunch<Line> lines;
    // Screen for displaying objects
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
                        points.push(Point(x, y, s));
                        break;
                    case 2 :
                        std::cout << "Input a b." << std::endl;
                        std::cin >> a >> b;
                        ellipses.push(Ellipse(x, y, a, b, s));
                        break;
                    case 3 :
                        std::cout << "Input n l." << std::endl;
                        std::cin >> a >> b;
                        polys.push(Polygon(x, y, a, b, s));
                        break;
                    case 4 :
                        std::cout << "Input xp yp." << std::endl;
                        std::cin >> a >> b;
                        lines.push(Line(x, y, a, b, s));
                    default: std::cout << "Invalid shape choice" <<std::endl;
                }
                break;
            // List Items
            case 2 :
                for (int i=0; i<points.get_itemCount(); i++)
                    std::cout << points[i].info();
                std::cout << std::endl;
                for (int i=0; i<ellipses.get_itemCount(); i++)
                    std::cout << ellipses[i].info();
                std::cout << std::endl;
                for (int i=0; i<polys.get_itemCount(); i++)
                    std::cout << polys[i].info();
                std::cout << std::endl;
                for (int i=0; i<lines.get_itemCount(); i++)
                    std::cout << lines[i].info();
                std::cout << std::endl;
                break;
            // Display chosen shape
            case 3 :
                std::cout << "Please choose from the following:\n";
                std::cout << "1. Point\n2. Ellipse\n3. Polygon\n4. Line\n5. All shapes\n";
                int opt_2; std::cin >> opt_2;
                int idx;
                if (opt_2 !=5){
                    std::cout << "Input shapes index in bunch" << std::endl;
                    std::cin >> idx;
                }
                switch (opt_2) {
                    case 1 :
                        display_shapes<Point>(points, idx, screen);
                        break;
                    case 2 :
                        display_shapes<Ellipse>(ellipses, idx, screen);
                        break;
                    case 3 :
                        display_shapes<Polygon>(polys, idx, screen);
                        break;
                    case 4 :
                        display_shapes<Line>(lines, idx, screen);
                        break;
                    case 5 :
                        draw_bunch(points, screen);
                        draw_bunch(lines, screen);
                        draw_bunch(ellipses, screen);
                        draw_bunch(polys, screen);
                        screen.display();
                        screen.clear();
                    default: std::cout << "Invalid shape choice" <<std::endl;
                }
                break;
            // Save / Load data
            case 4 :
                std::cout << "Choose the file: " << std::endl;
                std::string fname; std::cin >> fname;
                std::cout << "Choose 1 for save or 2 for load: ";
                int opt_3; std::cin >> opt_3;
                if (opt_3 == 1) {
                    store_data_to_file(points, fname);
                    store_data_to_file(lines, fname);
                    store_data_to_file(ellipses, fname);
                    store_data_to_file(polys, fname);
                } else {
                    load_file_data(points, lines, ellipses, polys, fname);
                }
                break;
//            default: if(option != -1) std::cout << "Not an option choose again\n" << std::endl;
        }
    }

}