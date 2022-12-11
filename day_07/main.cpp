#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <tuple>

struct Directory {
    Directory* parent = nullptr;
    std::vector<Directory*> childs;
    std::string name;
    unsigned int size = 0;
    ~Directory() {
        for(auto ptr : childs) delete ptr;
        childs.clear();
    }
};

enum class CommandType { NOC, CD, LS, FILE, DIR };

void recalculate_sizes(Directory* const start) {
    for(auto child : start->childs) {
        recalculate_sizes(child);
        start->size += child->size;
    }
}

static void part_one(Directory const* const start, unsigned int& part_one_sum) {
    if(start->size <= 100'000) part_one_sum += start->size;

    for(auto child : start->childs) {
        part_one(child, part_one_sum);
    }
}

static void part_two_rec(Directory const* const start, unsigned int unused_space, std::vector<unsigned int>& candidates) {
    static const int necessary_space = 30'000'000;

    if(start->size >= (necessary_space - unused_space)) candidates.push_back(start->size);

    for(auto child : start->childs) {
        part_two_rec(child, unused_space, candidates);
    }
}

static void part_two(Directory const* const start, unsigned int currently_used_space) {
    static const int available_space = 70'000'000;

    unsigned int unused_space = available_space - currently_used_space;

    std::vector<unsigned int> candidates;
    // find a directory to delete
    part_two_rec(start, unused_space, candidates);

    auto min = std::min_element(candidates.begin(), candidates.end());
    std::cout << "Min. element to free space: " << *min << std::endl;
}

using CommandTypeResult = std::tuple<CommandType, std::string>;

static CommandTypeResult get_command(std::string const& line) {
    if(line.starts_with("$ cd")) return std::make_tuple(CommandType::CD, line.substr(5));
    if(line.starts_with("$ ls")) return std::make_tuple(CommandType::LS, "");
    if(line.starts_with("dir ")) return std::make_tuple(CommandType::DIR, line.substr(4));
    
    // at this point it can only be a file
    auto ws_pos = line.find_first_of(' ');
    return std::make_tuple(CommandType::FILE, line.substr(0, ws_pos));
}

int main() {
  std::string line;
  std::ifstream file("input-final.txt");

  if (file.fail())
    return 1;

  CommandTypeResult next_command = std::make_tuple(CommandType::NOC, "");
  Directory* current_directory = new Directory({.name = "/"});
  Directory* root = current_directory;

  // ignore the first cd / line
  std::getline(file, line);
  while (std::getline(file, line)) {
      next_command = get_command(line);
      auto command_type = std::get<0>(next_command);
      std::string command_arg = std::get<1>(next_command);

      // if we do a ls we only can work with the next line(s)
      if(command_type == CommandType::LS) continue;

      // go one directory up
      if(command_type == CommandType::CD and command_arg == "..") {
          current_directory = current_directory->parent;
      } else if(command_type == CommandType::CD) {
          auto next_dir = std::find_if(current_directory->childs.begin(), current_directory->childs.end(), [&command_arg](Directory* dir){ return dir->name == command_arg; }); 
          if(next_dir == current_directory->childs.end()) std::cout << "nullptr\n";
          current_directory = *next_dir;
      }

      // create new child in the current directory
      if(command_type == CommandType::DIR) {
          auto dir = new Directory;
          dir->parent = current_directory;
          dir->name = command_arg;
          current_directory->childs.push_back(dir); 
      }

      // only add file size to the current directory (files are not from interest in this exercise) 
      if(command_type == CommandType::FILE) {
          auto size = std::stoul(command_arg);
          current_directory->size += size;
      } 
  }  

  recalculate_sizes(root);

  unsigned int part_one_sum{0};
  part_one(root, part_one_sum);

  std::cout << "Part one - total size of directories <= 100'000: " << part_one_sum << std::endl;

  part_two(root, root->size);

  for(auto ptr : root->childs) {
      delete ptr;
  }
  delete root;
}

