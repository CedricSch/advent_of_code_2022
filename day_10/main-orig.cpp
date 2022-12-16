#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <ranges>


int main() {
    std::vector<std::string> instructions;

    std::ifstream file("input-final.txt");

    if(file.fail()) return 1;

    std::string line;
    while(std::getline(file, line)) instructions.push_back(line);

    // holds information at which cycle which content is present in the register
    std::vector<std::tuple<unsigned int, int>> register_content{};
    register_content.push_back(std::make_tuple(1, 1));

    unsigned int last_cycle = 1;
    for(auto const& instruction_string : instructions) {
        if(instruction_string.starts_with("noop")) {
            last_cycle++;
            auto last_register_value = std::get<1>(register_content.back());
            register_content.push_back(std::make_tuple(last_cycle, last_register_value)); 
        } else {
            auto ws_pos = instruction_string.find_first_of(' ');
            std::string instruction_name = instruction_string.substr(0, ws_pos);
            int instruction_value = std::stoi(instruction_string.substr(ws_pos + 1));
            last_cycle += 2;
            auto last_register_value = std::get<1>(register_content.back());
            register_content.push_back(std::make_tuple((last_cycle-1), last_register_value)); 
            register_content.push_back(std::make_tuple(last_cycle, last_register_value + instruction_value)); 
        }
    }

    unsigned int signal_strength{0};
    auto cycles = register_content | std::views::filter([](auto const& t) {
        unsigned int cycle = std::get<0>(t);
        return cycle == 20 or cycle == 60 or cycle == 100 or cycle == 140 or cycle == 180 or cycle == 220;
    });

    for(auto const& t : cycles) {
        unsigned int cycle = std::get<0>(t);
        int content = std::get<1>(t);

        signal_strength += (cycle *= content);
    }


    if(signal_strength != 15680) std::cout << "Your signal strength is wrong!!!" << std::endl;
    else std::cout << "Signal strength: " << signal_strength << std::endl;
}
