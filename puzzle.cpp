#include "puzzle.hpp"
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <stdexcept>

bool Group::contains(char val) const{
    for (Cell* cell : cells_){
        if((*cell).val()==val) return true;
    }
    return false;
}

Grid::Grid(std::vector<std::vector<char>> values){
    std::size_t numrows = 0, numcols = 0, numboxes = 0;
    std::size_t boxwidth = 0, boxheight = 0;
    //determine allowed values, and numrows/numcols/numboxes
    switch (values.size()){
        case 4:  allowed_vals_ = {'1','2','3','4'};
                 numrows = numcols = numboxes = 4;
                 boxwidth = boxheight = 2;
                 break;
        case 6:  allowed_vals_ = {'1','2','3','4','5','6'};
                 numrows = numcols = numboxes = 6;
                 boxwidth = 3;
                 boxheight = 2;
                 break;
        case 9:  allowed_vals_ = {'1','2','3','4','5','6'};
                 numrows = numcols = numboxes = 9;
                 boxwidth = boxheight = 3;
                 break;
        case 16: allowed_vals_ = {'0','1','2','3','4','5','6','7','8','9',
                                  'A','B','C','D','E','F'};
                 numrows = numcols = numboxes = 16;
                 boxwidth = boxheight = 4;
                 break;
        default: throw std::length_error("Unknown puzzle size");
    }
    if (values[0].size()!=values.size())
        throw std::length_error("Unknown puzzle size");

    /* Create cells. Initialize cells_, cell_groups_, and cell_coordinates_, 
     * and fill rows, cols, and boxes.
     */
    std::vector<std::vector<Cell*>> rows(numrows);
    std::vector<std::vector<Cell*>> columns(numcols);
    std::vector<std::vector<Cell*>> boxes(numboxes);
    for (std::size_t row=0; row<numrows; row++){
        for (std::size_t col=0; col<numcols; col++){
            char val = values[row][col];
            cells_.push_back(Cell(val));
            cell_coordinates_[std::make_pair(row, col)] = &cells_.back();
            std::size_t box = boxheight*(row/boxheight) + (col/boxwidth);
            rows[row].push_back(&cells_.back());
            columns[col].push_back(&cells_.back());
            boxes[box].push_back(&cells_.back());
            cell_groups_[std::make_pair(row,col)] = {rows_[row],
                columns_[col], boxes_[box]};
        }
    }

    //initialize rows_, cols_, boxes_
    for (auto row: rows) rows_.push_back(Group(row));
    for (auto column: columns) columns_.push_back(Group(column));
    for (auto box: boxes) boxes_.push_back(Group(box));
}

std::set<char> Grid::possibilities(int x, int y) const{
    char cell_val = cell_coordinates_.at(std::make_pair(x,y))->val();
    std::set<char> candidates = allowed_vals_;
    std::vector<Group> groups = cell_groups_.at(std::make_pair(x,y));
    for(auto it = candidates.begin(); it!=candidates.end();){
        if (std::any_of(groups.begin(), groups.end(), 
                        [&](Group g){return g.contains(cell_val);}))
            it = candidates.erase(it);
        else ++it;
    } 
    return candidates;
}

