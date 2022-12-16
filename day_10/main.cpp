#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <ranges>
#include <map>

static void part_two(std::map<unsigned int, int> const& register_content) {
    for(auto [cycle, content] : register_content) {
        unsigned int normalized_cycle = (cycle -1) % 40;
        if(normalized_cycle == content-1 or normalized_cycle == content or normalized_cycle == content+1) std::cout << "#";
        else std::cout << ".";

        if(cycle % 40 == 0) std::cout << "\n";
    }
}

int main() {
    std::vector<std::string> instructions;

    std::ifstream file("input-final.txt");

    if(file.fail()) return 1;

    std::string line;
    while(std::getline(file, line)) instructions.push_back(line);

    // holds information at which cycle which content is present in the register
    std::map<unsigned int, int> register_content{};
    register_content[1] = 1;

    unsigned int last_cycle = 1;
    for(auto const& instruction_string : instructions) {
        if(instruction_string.starts_with("noop")) {
            auto last_register_value = register_content[last_cycle];
            last_cycle++;
            register_content[last_cycle] = last_register_value; 
        } else {
            auto ws_pos = instruction_string.find_first_of(' ');
            std::string instruction_name = instruction_string.substr(0, ws_pos);
            int instruction_value = std::stoi(instruction_string.substr(ws_pos + 1));
            auto last_register_value = register_content[last_cycle];
            last_cycle += 2;
            register_content[last_cycle-1] = last_register_value; 
            register_content[last_cycle] = last_register_value + instruction_value; 
        }
    }

    unsigned int signal_strength = 20 * register_content[20] + 60 * register_content[60] + 100 * register_content[100] + 
                                  140 * register_content[140] + 180 * register_content[180] + 220 * register_content[220];

    if(signal_strength != 15680) std::cout << "Your signal strength is wrong!!!" << std::endl;
    else std::cout << "Signal strength: " << signal_strength << std::endl;

    part_two(register_content);
}
