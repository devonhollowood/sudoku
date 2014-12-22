#ifndef SUDOKU_PUZZLE
#define SUDOKU_PUZZLE
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>

class Cell{
public:
    Cell(size_t row, size_t col, char val='.') : row_(row), col_(col), val_(val){}
    Cell() : Cell(0, 0, '.') {}
    char val() const {return val_;}
    void set(char val) {val_ = val;}
    size_t row() const {return row_;}
    size_t col() const {return col_;}
private:
    size_t row_;
    size_t col_;
    char val_;
};

class Group{
public:
    Group(const std::vector<Cell>& cells) : cells_(cells) {}
    bool contains(char val) const;
    std::vector<char> elements() const; 
    std::vector<Cell> cells() const {return(cells_);}
    Cell& operator[](size_t element) {return cells_[element];}
private:
    std::vector<Cell> cells_;
};

class Grid{
public:
    //constructors
    Grid(std::vector<std::vector<char>> values);
    Grid(std::vector<std::string> rows) : Grid(unpack_rows_(rows)) {}
    Grid(std::vector<Group> rows) : Grid(unpack_rows_(rows)) {}

    //cell Accessors
    Cell at(size_t row, size_t col) const;
    void set(size_t row, size_t col, char val);

    //group accessors
    std::vector<Group> all_groups() const;
    std::vector<Group> groups(size_t row, size_t col) const{
        return {rows()[row], columns()[col], boxes()[box(row,col)]};}
    std::vector<Cell> cells() const {return cells_;}
    std::vector<Group> rows() const;
    std::vector<Group> columns() const;
    std::vector<Group> boxes() const;
    
    //utility
    size_t side_length() const {return allowed_vals_.size();}
    const std::set<char>& allowed_vals() const {return allowed_vals_;}
    size_t box(size_t row, size_t col) const{
        return boxheight_*(row/boxheight_) + (col/boxwidth_);
    }
private:
    std::vector<std::vector<char>> unpack_rows_(std::vector<std::string> rows)
        const;
    std::vector<std::vector<char>> unpack_rows_(std::vector<Group> rows) const;
    std::vector<Cell> cells_;
    std::set<char> allowed_vals_;
    size_t boxheight_;
    size_t boxwidth_;
};

inline bool operator==(const Cell& lhs, const Cell& rhs){
    return lhs.val()==rhs.val() && lhs.row()==rhs.row() && lhs.col()==rhs.col();}
inline bool operator!=(const Cell& lhs, const Cell& rhs){return !(lhs==rhs);}
inline bool operator==(const Group& lhs, const Group& rhs){
    return lhs.elements()==rhs.elements();}
inline bool operator!=(const Group& lhs, const Group& rhs){return !(lhs==rhs);}
inline bool operator==(const Grid& lhs, const Grid& rhs){
    return lhs.cells()==rhs.cells();
}
inline bool operator!=(const Grid& lhs, const Grid& rhs){return !(lhs==rhs);}

#endif
