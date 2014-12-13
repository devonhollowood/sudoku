#include "parser.hpp"
#include "puzzle.hpp"
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>

Grid Parser::parse(std::vector<std::string> args){
    if(args.size() <=1){
        std::string error_str = "Wrong number of arguments.";
        std::invalid_argument error(error_str + '\n' + usage);
        throw(error);
    }
    else if(args[1]=="-f"){
        return from_file_(args[2]);
    }
    else{
        std::vector<std::string> rows(++args.begin(), args.end());
        return Grid(rows);
    }
}
    
Grid Parser::from_file_(std::string filename) const{
    std::ifstream filestream(filename);
    std::string line;
    std::vector<std::string> rows;
    while(std::getline(filestream, line)){ 
        line.erase(std::remove_if(line.begin(), line.end(), isspace), 
                std::end(line));
        rows.emplace_back(line);
    }
    return Grid(rows);
}
