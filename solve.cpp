#include "puzzle.hpp"
#include "parser.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <exception>

using namespace std;

void print(Grid g){
    for(auto row : g.rows()){
        for(auto element : row.elements()) cout << element << ' ';
        cout << endl;
    }
}

bool conflicts(std::vector<Group> groups, char c){
    return std::any_of(groups.begin(), groups.end(), 
                [c](Group g){return g.contains(c);});
}

std::set<char> possibilities(Grid g, size_t row, size_t col){
    if(g.at(row, col).val()!='.') return {g.at(row,col).val()};
    std::set<char> candidates;
    std::vector<Group> groups = g.groups(row, col);
    for(char val : g.allowed_vals()){
        if(!conflicts(groups, val)) candidates.insert(val);
    }
    return candidates;
}

Grid from_possibilities(const Grid& g){
    Grid next_gen(g);
    for(Cell c : g.cells()){
        size_t row = c.row();
        size_t col = c.col();
        //only consider cells that are currently empty
        if(g.at(row,col).val()!='.') continue;
        //find possibilities for cell
        auto candidates = possibilities(g, row, col);
        //if there are no possibilities, we've hit a dead end
        if(candidates.empty()){
            throw(std::runtime_error("No solutions"));
        }
        //if there's exactly one, we've found an easy solution
        if(candidates.size()==1){
            next_gen.set(row,col,*candidates.begin());
        }
    }
    return next_gen;
}

vector<Cell> possible_locations(const Grid& g, const Group& group, char val){
    vector<Cell> possible_locations;
    for(Cell cell : group.cells()){
        if(cell.val()==val) return {cell}; //val can only be in one cell
        if(cell.val()!='.') continue; //don't have to consider non-empties
        //add cell if its possible val goes there
        auto cell_possibilities = possibilities(g, cell.row(), cell.col());
        if(cell_possibilities.find(val)!=cell_possibilities.end()){
            possible_locations.push_back(cell);
        }
    }
    //return possible locations for val
    return possible_locations;
}

Grid from_necessity(const Grid& g){
    //find values that can only go one place
    Grid next_gen(g);
    for(Group group : g.all_groups()){
        for(char c : g.allowed_vals()){
            auto possible_locs = possible_locations(g, group, c);
            if(possible_locs.size()==0) 
                throw(std::runtime_error("No solutions"));
            if(possible_locs.size()==1){
                Cell cell = possible_locs[0];
                next_gen.set(cell.row(), cell.col(), c);
            }
        }
    }
    return next_gen;
}

Grid brute_force(const Grid& g){
    Grid solve(const Grid& g); //forward declare
    for(size_t row=0; row<g.side_length(); row++){
        for(size_t col=0; col<g.side_length(); col++){
            //only consider cells that are currently empty
            if(g.at(row,col).val()!='.') continue;
            for(auto candidate : possibilities(g,row,col)){
                Grid next_gen(g); //reset each time
                try{
                    next_gen.set(row,col,candidate);
                    return solve(next_gen);
                }
                catch(std::runtime_error){
                    continue;
                }
            }
        }
    }
    throw(std::runtime_error("No solutions."));
}

Grid solve(const Grid& g){
    auto cells = g.cells();
    //see if Cells are left to solve:
    size_t remaining = std::count_if(cells.begin(), cells.end(),
            [](Cell c){return c.val()=='.';});
    
    //base case: none are left to solve
    if(!remaining) return g;

    //Not solved, so find easy squares
    Grid next_gen = from_possibilities(g);
    next_gen = from_necessity(next_gen);

    //if you found any easy solutions, go one deeper
    if(next_gen!=g) return solve(next_gen);
    
    //finally, if you did not find any easy solutions, brute force
    cout << "Brute forcing :(" << endl; //debug
    return brute_force(g);
}

int main(int argc, char** argv){
    Parser p;
    std::vector<std::string> args(argv, argv+argc);
    Grid puzzle = p.parse(args);
    cout << "Input: " << endl;
    print(puzzle);
    cout << endl; 
    try{
        Grid solved_puzzle = solve(puzzle);
        cout << "Solved puzzle: " << endl;
        print(solved_puzzle);
    }
    catch(std::runtime_error e){
        cout << e.what() << endl;
        exit(1);
    }
}
