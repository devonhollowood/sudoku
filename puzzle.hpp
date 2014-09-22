#ifndef SUDOKU_PUZZLE
#define SUDOKU_PUZZLE
#include <vector>
#include <map>
#include <set>
#include <utility>


class Cell{
public:
    Cell(char val='.') : val_(val){}
    char val() const {return val_;}
    void set(char val) {val_ = val;}
private:
    char val_;
};

class Group{
public:
    Group(const std::vector<Cell*>& cells) : cells_(cells) {}
    bool contains(char val) const;
private:
    std::vector<Cell*> cells_;
};

class Grid{
public:
    Grid(std::vector<std::vector<char>> values);
    std::set<char> possibilities(int x, int y) const;
    std::vector<Cell> cells() const {return cells_;}
    std::vector<Group> rows() const {return rows_;}
    std::vector<Group> columns() const {return columns_;}
    std::vector<Group> boxes() const {return boxes_;}
private:
    std::map<std::pair<std::size_t,std::size_t>, std::vector<Group>> 
        cell_groups_;
    std::map<std::pair<std::size_t,std::size_t>, Cell*> cell_coordinates_;
    std::vector<Group> rows_;
    std::vector<Group> columns_;
    std::vector<Group> boxes_;
    std::vector<Cell> cells_;
    std::set<char> allowed_vals_;
};

#endif
