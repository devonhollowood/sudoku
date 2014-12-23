#include "fastgrid.hpp"
#include "fastsolver.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <array>
#include <cctype>
#include <stdexcept>

using namespace std;

std::vector<char> read_file(std::string filename){
    std::vector<char> cells;
    std::ifstream file(filename);
    if(!file){ throw(invalid_argument("Invalid file")); }
    char c;
    while(file.get(c)){
        if(c=='.' || isalnum(c)) cells.push_back(c);
    }
    return cells;
}

int main(int argc, char** argv){
    //read file
    std::string usage = "Usage: fastsolve [filename]";
    std::vector<char> cells;
    bool verbose=false;
    switch(argc){
        case 3:
            if (string(argv[1]) == "-v"){
                verbose=true;
            }
            else{
                cout << "Unrecognized argument: " << argv[1] << "."
                    << usage << endl;
                exit(1);
            }
            try{
                cells = read_file(argv[2]);
            }
            catch(invalid_argument e){
                cout << e.what() << ' ' << usage << endl;
            }
            break;
        case 2:
            try{
                cells = read_file(argv[1]);
            }
            catch(invalid_argument e){
                cout << e.what() << ' ' << usage << endl;
            }
            break;
        default:
            cout << "Invalid # of args. " << usage << endl; 
            exit(1);
            break;
    }
    //solve
    switch(cells.size()){
        case 16:{
            std::array<char, 16> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 4> allowed_vals = {{'1','2','3','4'}};
            FastGrid<4,2,2> grid(cells_array);
            Solver<4,2,2>(grid, allowed_vals, verbose).solve();
            break;
        }
        case 36:{
            std::array<char, 36> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 6> allowed_vals = 
                {{'1','2','3','4','5','6'}};
            FastGrid<6,2,3> grid(cells_array);
            Solver<6,2,3>(grid, allowed_vals, verbose).solve();
            break;
        }
        case 81:{
            std::array<char, 81> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 9> allowed_vals = 
                {{'1','2','3','4','5','6','7','8','9'}};
            FastGrid<9,3,3> grid(cells_array);
            Solver<9,3,3>(grid, allowed_vals, verbose).solve();
            break;
        }
        case 256:{
            std::array<char, 256> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 16> allowed_vals = 
                {{'1','2','3','4','5','6','7','8','9','0',
                     'A','B','C','D','E','F'}};
            FastGrid<16,4,4> grid(cells_array);
            Solver<16,4,4>(grid, allowed_vals, verbose).solve();
            break;
        }
        default:{
            cout << "Unrecognized puzzle size. " 
                << "Allowed sizes are 4x4, 6x6, 9x9, and 16x16."
                << endl;
            exit(1);
        }
    }
}
