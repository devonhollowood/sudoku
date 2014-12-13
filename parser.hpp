#ifndef SUDOKU_PARSE
#define SUDOKU_PARSE
#include "puzzle.hpp"
#include <string>
#include <vector>
#include <exception>


class Parser{
public:
    Grid parse(std::vector<std::string> args);
private:
    Grid from_file_(std::string filename) const;
    constexpr static auto usage = 
        "usage: solve [-f file | [row1 row2 row3 ...]]";
};
#endif
