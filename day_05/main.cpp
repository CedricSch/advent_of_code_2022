#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <map>

struct instruction {
    int from_stack;
    int to_stack;
    int amount;
};

static void parse_crates(std::string const& line, std::map<unsigned int, std::vector<unsigned char>>& crates) {
    int space_count{0};
    int stack_counter{1};
    static int whitespace_stride{3};

    for(std::size_t i = 0; i < line.size(); i++) {
        if(line[i] == ']') {
            crates[stack_counter].insert(crates[stack_counter].begin(),line[i-1]);
            stack_counter++;
            space_count = 0;
        }
        else if(line[i] == ' ' and space_count == whitespace_stride) {
            stack_counter++;
            space_count = 0;
        } else {
            space_count++;
        }
    }
}

static void parse_instructions(std::string const& line, std::vector<instruction>& instructions) {
    int amount, from, to;
    std::string trash{};

    std::istringstream ss(line); 

    ss >> trash >> amount >> trash >> from >> trash >> to;
    instructions.push_back({.from_stack = from, .to_stack = to, .amount = amount});
}

static void apply_instructions_part_one(std::vector<instruction> const& instructions, std::map<unsigned int, std::vector<unsigned char>>& crates) {
    for(auto instruction : instructions) {
        auto& from_stack = crates[instruction.from_stack];
        auto& to_stack = crates[instruction.to_stack];

        for(std::size_t i{0}; i < instruction.amount; i++) {
            to_stack.push_back(from_stack.back());
            from_stack.pop_back();
        }
    }
}

static void apply_instructions_part_two(std::vector<instruction> const& instructions, std::map<unsigned int, std::vector<unsigned char>>& crates) {
    for(auto instruction : instructions) {
        auto& from_stack = crates[instruction.from_stack];
        auto& to_stack = crates[instruction.to_stack];

        for(auto ptr = from_stack.cbegin() + from_stack.size() - instruction.amount; ptr != from_stack.cend(); ptr++) {
            to_stack.push_back(*ptr);
        }

        for(std::size_t i{0}; i < instruction.amount; i++) {
            from_stack.pop_back();
        }
    }
}

int main() {
  std::string line;
  std::ifstream file("input.txt");
  std::map<unsigned int, std::vector<unsigned char>> crates{};
  std::vector<instruction> instructions{};

  if (file.fail())
    return 1;

  bool read_instructions{false};
  while (std::getline(file, line)) {
      if(line.empty()) continue;

      if(auto ptr = line.find_first_not_of(' '); ptr != std::string::npos and line[ptr] == '1') {
          read_instructions = true;
      } else if(read_instructions) {
          parse_instructions(line, instructions);
      } else {
          parse_crates(line, crates);
      }
  }

  apply_instructions_part_two(instructions, crates);

  std::string message(crates.size(), ' ');
  int counter{0};
  for(auto const& [key, value] : crates) {
     message[counter++] = value.back();
  }

  std::cout << "Message: " << message << std::endl;
}
