#ifndef SUDOKU_FASTGRID
#define SUDOKU_FASTGRID
#include <array>
#include <vector>
#include <string>
#include <functional>
#include <utility>

template <size_t side_length, size_t box_height, size_t box_width> class FastGrid{
public:
    typedef std::array<char, side_length> Group;
    FastGrid(std::array<char, side_length*side_length> cells) : 
        cells_(cells) {}
    std::array<char, side_length*side_length> cells() const{return cells_;}
    Group row(size_t r) const{
        return group_([r](size_t i){return r*side_length + i;});
    }
    Group col(size_t c) const{
        return group_([c](size_t i){return i*side_length + c;});
    }
    Group box(size_t b) const{
        auto boxes_per_row = side_length/box_width;
        auto top_left = (b/boxes_per_row)*box_height*side_length 
            + (b%boxes_per_row)*box_width;
        return group_([b, top_left, this](size_t i){
                return top_left + (i/box_width)*side_length + (i%box_width);
            });
    }
    std::array<Group, side_length> rows() const{
        std::array<Group, side_length> result;
        for(size_t i=0; i<side_length; ++i) result[i]=row(i);
        return result;
    }
    std::array<Group, side_length> cols() const{
        std::array<Group, side_length> result;
        for(size_t i=0; i<side_length; ++i) result[i]=col(i);
        return result;
    }
    std::array<Group, side_length> boxes() const{
        std::array<Group, side_length> result;
        for(size_t i=0; i<side_length; ++i) result[i]=box(i);
        return result;
    }
    size_t box_num(size_t r, size_t c) const{
        return box_height*(r/box_height) + (c/box_width);
    }
    std::array<Group, 3> groups(size_t r, size_t c) const{
        return {row(r), col(c), box(box_num(r, c))};
    }
    std::array<Group, 3*side_length> all_groups() const{
        std::array<Group, 3*side_length> result;
        auto all_rows = rows();
        auto all_cols = cols();
        auto all_boxes = boxes();
        std::copy(all_rows.begin(), all_rows.end(), result.begin());
        std::copy(all_cols.begin(), all_cols.end(), 
                result.begin() + side_length);
        std::copy(all_boxes.begin(), all_boxes.end(), 
                result.begin() + 2*side_length);
        return result; 
    }
    char at(size_t r, size_t c) const{
        return cells_[r*side_length + c];
    }
    void set(size_t r, size_t c, char val){
        cells_[r*side_length + c] = val;
    }
private:
    std::array<char, side_length*side_length> cells_;
    Group group_(std::function<size_t(size_t)> translation) const{
        Group result;
        for(size_t i=0; i<side_length; ++i)
            result[i] = cells_[translation(i)];
        return result;
    }
};

#endif
