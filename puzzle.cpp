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
}
    
Cell Grid::at(size_t row, size_t col) const{
    return cells_[row*side_length() + col];
}

void Grid::set(size_t row, size_t col, char val){
    cells_[side_length()*row + col].set(val);
}

std::vector<Group> Grid::all_groups() const{
    std::vector<Group> groups;
    std::vector<Group> all_rows=rows(), all_columns=columns(), 
        all_boxes=boxes();
    groups.insert(groups.end(), all_rows.begin(), all_rows.end());
    groups.insert(groups.end(), all_columns.begin(), all_columns.end());
    groups.insert(groups.end(), all_boxes.begin(), all_boxes.end());
    return groups;
}
    
std::vector<Group> Grid::rows() const{
    std::vector<std::vector<Cell>> rows(side_length());
    for(Cell c : cells_){
        rows[c.row()].push_back(c);
    }
    std::vector<Group> results;
    for(auto row : rows) results.push_back(Group(row));
    return results;
}
std::vector<Group> Grid::columns() const{
    std::vector<std::vector<Cell>> cols(side_length());
    for(Cell c : cells_){
        cols[c.col()].push_back(c);
    }
    std::vector<Group> results;
    for(auto col : cols) results.push_back(Group(col));
    return results;
}
std::vector<Group> Grid::boxes() const{
    std::vector<std::vector<Cell>> boxes(side_length());
    for(Cell c : cells_){
        boxes[box(c.row(), c.col())].push_back(c);
    }
    std::vector<Group> results;
    for(auto box : boxes) results.push_back(Group(box));
    return results;
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

