#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Node {
    enum class Type { FILE, DIRECTORY };
    Type type = Type::DIRECTORY;
    Node* parent = nullptr;
    std::vector<Node*> childs;
    std::string name;
    unsigned int size = 0;
};

struct Command {
    enum class Type { CD, LS, NOC };
    Type type = Type::NOC;
    std::string arg;
};

static Node* get_node(std::string const& line) {
    Node* node = new Node;
    if(line.starts_with("dir")) {
        node->name = line.substr(4);
    } else {
        node->type = Node::Type::FILE;
        auto ws_pos = line.find_first_of(' ');
        node->size = std::stoul(line.substr(0, ws_pos));
        node->name = line.substr(ws_pos + 1);
    }

    return node;
}

void print_filesystem(Node* const start, int indent, int& counter) {
    if(start->childs.size() == 0) return;

    for(auto child : start->childs) {
        print_filesystem(child, indent+1, counter);
        start->size += child->size;
    }

    std::string indent_str(indent, ' ');
    if(start->type == Node::Type::DIRECTORY and start->size <= 100'000) {
        counter += start->size;
        std::cout << indent_str << "[Dir] " << start->name << ", size: " << start->size << " \n";
    } else {
        //std::cout << indent_str << "[File] " << start->name << ", size: " << start->size << " \n";
    }
}

static Command get_command(std::string const& line) {
    Command cmd;
    if(line.starts_with("cd")) {
        cmd.type = Command::Type::CD;
        // cd<whitespace> = 3
        cmd.arg = line.substr(3);
    } else {
        cmd.type = Command::Type::LS;
    }

    return cmd;
}

int main() {
  std::string line;
  std::ifstream file("input-final.txt");

  if (file.fail())
    return 1;

  Command last_command{};
  Node* current_directory = new Node;
  current_directory->name = "/";
  Node* root = current_directory;

  std::getline(file, line);
  while (std::getline(file, line)) {
      // Command
      if(line.starts_with('$')) {
        last_command = get_command(line.substr(2));
        //std::cout << "Last command: " << (last_command.type == Command::Type::CD ? "cd" : "ls") << " with arg: " << (last_command.type == Command::Type::LS ? "<none>" : last_command.arg) << std::endl;
        if(last_command.type == Command::Type::LS) continue;
      } 
      
      if(last_command.type == Command::Type::CD) {
          if(last_command.arg == "..") {
              std::cout << "===.. " << current_directory->name << " ---> " << current_directory->parent->name << " ===\n";
              current_directory = current_directory->parent;
          } else {
              auto ptr = std::find_if(current_directory->childs.begin(), current_directory->childs.end(), [last_command](auto* ptr){ return last_command.arg == ptr->name and ptr->type == Node::Type::DIRECTORY; });
              if(ptr == current_directory->childs.end()) {
                  std::cout << "NULLPTR\n";
              }
              std::cout << "=== " << current_directory->name << " ---> " << (*ptr)->name << " ===\n";
              current_directory = *ptr;
          }
      } else {
          auto* node = get_node(line);
          //std::cout << (node->type == Node::Type::FILE ? "File" : "Directory") << " with name: " << node->name << " and size: " << node->size << std::endl;
          node->parent = current_directory;
          current_directory->childs.push_back(node);
      }
  }
  int counter{0};
  print_filesystem(root, 0, counter);
  std::cout << "Combined size: " << counter << std::endl;
}
