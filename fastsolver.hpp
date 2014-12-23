#ifndef SUDOKU_FASTSOLVER
#define SUDOKU_FASTSOLVER

#include "fastgrid.hpp"
#include <array>
#include <set>
#include <algorithm>
#include <iostream>

template<size_t side_length, size_t box_height, size_t box_width>
class Solver{
public:
    typedef FastGrid<side_length, box_height, box_width> GridType;
    Solver(const GridType& g, std::array<char, side_length> allowed_vals,
            bool verbose=false) : grid_(g), allowed_vals_(allowed_vals), 
        verbose_(verbose) {initialize_possiblities_();}
    void solve(bool recursed=false){
        if(!recursed){
            std::cout << "Initial puzzle:" << std::endl;
            print_();
            std::cout << std::endl;
        }
        while(!solved_()){
            if(from_possibilities_() || from_necessity_()){
                if(verbose_){
                    print_();
                    std::cout << std::endl;
                }
            }
            else{
                std::cout << "brute forcing :-(" << std::endl;
                brute_force_();
            }
        }
        if(!recursed){ 
            std::cout << "Solved puzzle:" << std::endl;
            print_();
        }
    }
private:
    typedef std::array<std::array<std::set<char>, side_length>, side_length> 
        PossibilityArray;
    GridType grid_;
    std::array<char, side_length> allowed_vals_;
    bool verbose_;
    PossibilityArray possibilities_;
    bool solved_() const{
        auto cells = grid_.cells();
        auto filled = [](char c){return c!='.';};
        return std::all_of(cells.begin(), cells.end(), filled);
    }
    bool conflicts_(size_t r, size_t c, char val) const{
        auto groups = grid_.groups(r,c);
        for(auto group : groups){
            if(std::find(group.begin(), group.end(), val)!=group.end()) 
                return true;
        }
        return false;
    }
    std::set<char> calculate_possibilities_(size_t r, size_t c) const{
        if(grid_.at(r,c) !='.') return {grid_.at(r,c)};
        std::set<char> candidates;
        for(char val : allowed_vals_){
            if(!conflicts_(r,c,val)) candidates.insert(val);
        }
        return candidates;
    }
    void initialize_possiblities_(){
        for(size_t r=0; r<side_length; ++r){
            for(size_t c=0; c<side_length; ++c){
                possibilities_[r][c] = calculate_possibilities_(r,c);
            }
        }
    }
    void set_(size_t r, size_t c, size_t val){
        grid_.set(r,c,val);
        size_t b = grid_.box_num(r,c);
        for(size_t i=0; i<side_length; ++i){ 
            auto row_coords = coords_from_row_(r,i);
            possibilities_[row_coords.first][row_coords.second].erase(val);
            auto col_coords = coords_from_col_(c,i);
            possibilities_[col_coords.first][col_coords.second].erase(val);
            auto box_coords = coords_from_box_(b,i);
            possibilities_[box_coords.first][box_coords.second].erase(val);
        }
        possibilities_[r][c].clear();
        possibilities_[r][c].insert(val);
    }
    void print_() const{
        for(size_t r=0; r<side_length; ++r){
            //print horizontal bars
            if(!(r%box_height) && r!=0){
                size_t num_bars = side_length/box_width-1;
                size_t num_chars = side_length;
                size_t num_spaces = num_bars+num_chars-1;
                size_t line_length = num_bars + num_chars + num_spaces;
                for(size_t c=0; c<line_length; ++c) std::cout << '-';
                std::cout << std::endl;
            }
            //print others
            for(size_t c=0; c<side_length; ++c){
                if(!(c%box_width) && c!=0) std::cout << "| ";
                std::cout << grid_.at(r,c);
                if(c!=side_length-1) std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }
    bool from_possibilities_(){
        bool changed = false;
        for(size_t r=0; r<side_length; ++r){
            for(size_t c=0; c<side_length; ++c){
                //only consider cells that are currently empty
                if(grid_.at(r,c)!='.') continue;
                //find possibilities for cell
                auto candidates = possibilities_[r][c];
                //if there are no possibilities, we've hit a dead end
                if(candidates.empty()){
                    std::string coord = "(" + std::to_string(r)
                        + "," + std::to_string(c) + ")";
                    std::string error = "No solutions for " + coord + ".";
                    throw(std::runtime_error(error));
                }
                //if there's exactly one, we've found an easy solution
                if(candidates.size()==1){
                    auto val = *candidates.begin();
                    if(verbose_){
                        std::cout << '(' << r << ',' << c << ')'
                            << " can only be " << val << std::endl; 
                    }
                    set_(r,c,val);
                    changed = true;
                }
            }
        }
        return changed;
    }
    std::set<char> needs_(typename GridType::Group group){
        std::set<char> needed_vals;
        for(char c : allowed_vals_){
            if(std::find(group.begin(), group.end(), c)==group.end()){
                needed_vals.insert(c);
            }
        }
        return needed_vals;
    }
    std::pair<size_t, size_t> coords_from_row_(size_t r, size_t i) const{
        return std::make_pair(r,i);
    }
    std::pair<size_t, size_t> coords_from_col_(size_t c, size_t i) const{
        return std::make_pair(i,c);
    }
    std::pair<size_t, size_t> coords_from_box_(size_t b, size_t i) const{
        auto boxes_per_row = side_length/box_width;
        auto r = (b/boxes_per_row)*box_height + i/box_width;
        auto c = (b%boxes_per_row)*box_width + i%box_width;
        return std::make_pair(r,c);
    }
    std::pair<size_t, size_t> coords_from_group_(size_t g, size_t i) const{
        switch(g/side_length){
            case 0: //row
                return coords_from_row_(g%side_length, i);
            case 1: //col
                return coords_from_col_(g%side_length, i);
            default: //box
                return coords_from_box_(g%side_length, i);
        }
    }
    bool from_necessity_(){
        bool changed = false;
        //loop through groups
        auto all_groups = grid_.all_groups();
        for(size_t g=0; g<all_groups.size(); ++g){
            //loop through needed vals for each group
            auto needed_vals = needs_(all_groups[g]);
            for(char val : needed_vals){
                //find places they can go
                std::vector<size_t> possible_locs;
                for(size_t p=0; p<side_length; ++p){
                    size_t r = coords_from_group_(g,p).first;
                    size_t c = coords_from_group_(g,p).second;
                    if(possibilities_[r][c].count(val)) 
                        possible_locs.push_back(p);
                }
                //if they can only go in one place, add them
                if(possible_locs.size()==1){
                    size_t p = possible_locs.front();
                    size_t r = coords_from_group_(g,p).first;
                    size_t c = coords_from_group_(g,p).second;
                    if(verbose_){
                        std::string grouptype;
                        switch(g/side_length){
                            case 0: grouptype="Row"; break;
                            case 1: grouptype="Col"; break;
                            default: grouptype="Box"; break;
                        }
                        std::cout << grouptype << ' ' << g%side_length
                            << " needs " << val << ", can only go at "
                            << '(' << r << ',' << c << ')' << std::endl;
                    }
                    set_(r,c,val);
                    changed = true;
                }
            }
        }
        return changed;
    }
    void brute_force_(){
        //find element with least possibilities
        size_t min_possibililies = side_length+1; //more than max possible
        size_t min_r, min_c; 
        for(size_t r=0; r<side_length; ++r){
            for(size_t c=0; c<side_length; ++c){
                size_t num_possibilities = possibilities_[r][c].size();
                if(num_possibilities == 1) continue;
                if(num_possibilities < min_possibililies){
                    min_r = r; 
                    min_c = c;
                    min_possibililies = num_possibilities;
                }
            }
        }
        //save current state
        auto current_grid = grid_;
        auto current_possibilities = possibilities_;
        //try out all possibilities
        for(auto possibility : current_possibilities[min_r][min_c]){
            try{ //make change and try to solve new puzzle
                set_(min_r,min_c,possibility);
                if(verbose_){
                    std::cout << "Trying " << possibility << " at "
                        << '(' << min_r << "," << min_c << ')' << std::endl;
                    print_();
                    std::cout << std::endl;
                }
                solve(true);
                break;
            }
            catch(std::runtime_error e){ //reset
                grid_ = current_grid;
                possibilities_ = current_possibilities;
                if(verbose_){
                    std::cout << "Contradiction found: " << e.what() 
                        << std::endl;
                    std::cout << "Backing up to:" << std::endl;
                    print_();
                    std::cout << std::endl;
                }
            }
        }
        if(!solved_()) throw std::runtime_error("No solutions.");
    }
};

#endif
