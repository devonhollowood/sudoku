#include "fastgrid.hpp"
#include "fastsolver.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <array>
#include <cctype>

using namespace std;

int main(int argc, char** argv){
    //read file
    std::string usage = "usage: fastsolve [filename]";
    if(argc!=1){ cout << usage << endl; exit(1); }
    std::ifstream file(argv[1]);
    if(!file){ cout << usage << endl; exit(1); }
    char c;
    std::vector<char> cells;
    while(file.get(c)){
        if(isalnum(c)) cells.push_back(c);
    }
    //solve
    switch(cells.size()){
        case 16:{
            std::array<char, 16> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 4> allowed_vals = {{'1','2','3','4'}};
            FastGrid<4,2,2> grid(cells_array);
            Solver<4,2,2,allowed_vals>(grid).solve()
            break;
        }
        case 36:{
            std::array<char, 36> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 6> allowed_vals = 
                {{'1','2','3','4','5','6'}};
            FastGrid<6,2,3> grid(cells_array);
            Solver<6,2,3,allowed_vals>(grid).solve()
            break;
        }
        case 81:{
            std::array<char, 81> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 9> allowed_vals = 
                {{'1','2','3','4','5','6','7','8','9'}};
            FastGrid<9,3,3> grid(cells_array);
            Solver<9,3,3,allowed_vals>(grid).solve()
            break;
        }
        case 256:{
            std::array<char, 256> cells_array;
            std::copy(cells.cbegin(), cells.cend(), cells_array.begin());
            std::array<char, 16> allowed_vals = 
                {{'1','2','3','4','5','6','7','8','9','0',
                     'A','B','C','D','E','F'}};
            FastGrid<16,4,4> grid(cells_array);
            Solver<16,4,4,allowed_vals>(grid).solve()
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
