#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main() {
    std::vector<std::string> tree_map = {};

    std::ifstream file("./input/input_day08.txt");

    if(file.fail()) return 1;

    std::string line;
    while(std::getline(file, line)) tree_map.push_back(line);


    // init with the ammount of edge trees
    auto total_trees_visible{2 * tree_map.size() + 2 * (tree_map[0].size() - 2)};

    unsigned int max_scenic_score = 1;
    for(std::size_t row{1}; row < tree_map.size() - 1; row++) {
       for(std::size_t col{1}; col < tree_map[row].size() - 1; col++) {
           // convert char to uint
           unsigned int current_tree = tree_map[row][col] - '0';
           // this is so smart
           unsigned char is_tree_visible = 0b1111;
           unsigned int current_scenic_score = 1;

           // left
           for(int i = col - 1; i >= 0; i--) {
               unsigned int tree = tree_map[row][i] - '0';
               if(tree >= current_tree) {
                   current_scenic_score *= (col - i);
                   is_tree_visible &= 0b1110;
                   break;
               } else if(i == 0) current_scenic_score *= (col - i);
           }

           // right
           for(int i = col + 1; i < tree_map[row].size(); i++) {
               unsigned int tree = tree_map[row][i] - '0';
               if(tree >= current_tree) {
                   current_scenic_score *= (i - col);
                   is_tree_visible &= 0b1101;
                   break;
               } else if( (i+1) == tree_map[row].size()) current_scenic_score *= (i - col);
           }

           // top
           // holy shit DON'T USE UNSIGNED INTs !!!!!! i has to be negative so that i >= 0 can evaluate to false.
           // if i is unsigned it will wrap around and the loop never terminates !!!!
           for(int i = row - 1; i >= 0; i--) {
               unsigned int tree = tree_map[i][col] - '0';
               if(tree >= current_tree) {
                   current_scenic_score *= (row - i);
                   is_tree_visible &= 0b1011;
                   break;
               } else if(i == 0) current_scenic_score *= (row - i);
           }
 
           // bottom
           for(int i = row + 1; i < tree_map.size(); i++) {
               unsigned int tree = tree_map[i][col] - '0';
               if(tree >= current_tree) {
                   current_scenic_score *= (i - row);
                   is_tree_visible &= 0b0111;
                   break;
               } else if( (i+1) == tree_map.size()) current_scenic_score *= (i - row);
           }

           // can be seen at least from one direction
           if(is_tree_visible) total_trees_visible++;
           if(current_scenic_score > max_scenic_score) max_scenic_score = current_scenic_score;
       }
    }
    
    std::cout << "Part one: " << total_trees_visible << std::endl;
    std::cout << "Part two: " << max_scenic_score << std::endl;
}
