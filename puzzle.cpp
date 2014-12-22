#include "puzzle.hpp"
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <stdexcept>

/*
 * Group Functions
 */

bool Group::contains(char val) const{
    return std::any_of(cells_.begin(), cells_.end(), 
           [this, val](Cell c){return c.val()==val;});
}

std::vector<char> Group::elements() const{
    std::vector<char> res(cells_.size());
    std::transform(cells_.cbegin(), cells_.cend(), res.begin(),
            [](Cell c){return c.val();});
    return res;
}

/*
 *  Grid Functions
 */

Grid::Grid(std::vector<std::vector<char>> values){
    std::size_t numrows = 0, numcols = 0, numboxes = 0;
    //determine allowed values, and numrows/numcols/numboxes
    switch (values.size()){
        case 4:  allowed_vals_ = {'1','2','3','4'};
                 numrows = numcols = numboxes = 4;
                 boxwidth_ = boxheight_ = 2;
                 break;
        case 6:  allowed_vals_ = {'1','2','3','4','5','6'};
                 numrows = numcols = numboxes = 6;
                 boxwidth_ = 3;
                 boxheight_ = 2;
                 break;
        case 9:  allowed_vals_ = {'1','2','3','4','5','6', '7', '8', '9'};
                 numrows = numcols = numboxes = 9;
                 boxwidth_ = boxheight_ = 3;
                 break;
        case 16: allowed_vals_ = {'0','1','2','3','4','5','6','7','8','9',
                                  'A','B','C','D','E','F'};
                 numrows = numcols = numboxes = 16;
                 boxwidth_ = boxheight_ = 4;
                 break;
        default: throw std::length_error("Unknown puzzle size");
    }
    if (values[0].size()!=values.size())
        throw std::length_error("Unknown puzzle size");

    /* Create Cells. Fill cells_, rows, cols, and boxes.
     */
    std::vector<std::vector<Cell>> rows(numrows);
    std::vector<std::vector<Cell>> columns(numcols);
    std::vector<std::vector<Cell>> boxes(numboxes);
    for (std::size_t row=0; row<numrows; row++){
        for (std::size_t col=0; col<numcols; col++){
            char val = values[row][col];
            Cell cell(row, col, val);
            cells_.push_back(cell);
            rows[row].emplace_back(cell);
            columns[col].emplace_back(cell);
            boxes[box(row, col)].emplace_back(cell);
        }
    }

    //initialize rows_, cols_, boxes_
    for (auto row: rows) rows_.push_back(Group(row));
    for (auto column: columns) columns_.push_back(Group(column));
    for (auto box: boxes) boxes_.push_back(Group(box));
}
    
Cell Grid::at(size_t row, size_t col) const{
    return cells_[row*side_length() + col];
}

void Grid::set(size_t row, size_t col, char val){
    cells_[side_length()*row + col].set(val);
    rows_[row][col].set(val);
    columns_[col][row].set(val);
    boxes_[box(row,col)][row%boxheight_*boxwidth_ + col%boxwidth_].set(val);
}

std::vector<Group> Grid::all_groups() const{
    std::vector<Group> groups;
    groups.insert(groups.end(), rows_.begin(), rows_.end());
    groups.insert(groups.end(), columns_.begin(), columns_.end());
    groups.insert(groups.end(), boxes_.begin(), boxes_.end());
    return groups;
}
    
std::vector<std::vector<char>> Grid::unpack_rows_(
        std::vector<std::string> rows) const{
    std::vector<std::vector<char>> char_rows;  
    for(std::string row : rows){
        std::vector<char> char_row(row.begin(), row.end());
        char_rows.push_back(char_row);
    }
    return char_rows;
}

std::vector<std::vector<char>> Grid::unpack_rows_(std::vector<Group> rows) 
    const{
    std::vector<std::vector<char>> char_rows;  
    for(Group row : rows){
        std::vector<char> char_row;
        for(char c : row.elements()) char_row.push_back(c);
        char_rows.push_back(char_row);
    }
    return char_rows;
}

