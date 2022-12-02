#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

int main() {
    std::vector<long> elve_calories{};
    long elve_calorie_counter{0};

    std::string line;
    std::ifstream file("data.txt");

    if(file.fail()) return 1;
     
    while(std::getline(file, line)) {
        if(line.empty()) {
            // new elve in next line
            elve_calories.push_back(elve_calorie_counter);
            elve_calorie_counter = 0;
            continue;
        }

        try {
            elve_calorie_counter += std::stol(line);
        } catch(std::invalid_argument const& ex) {
            std::cout << "Unable to convert: " + line + " to an integer." << std::endl;
            return 1;
        } catch(std::out_of_range const& ex) {
            std::cout << "Integer: " + line + " out of range." << std::endl;
            return 1;
        }
    }

    // Get the three elves with the most calories
    // This code is not overcomplicated :^D
    std::tuple<std::size_t, long> elve_1{};
    std::tuple<std::size_t, long> elve_2{};
    std::tuple<std::size_t, long> elve_3{};
    
    for(std::size_t elve{0}; elve < elve_calories.size(); elve++) {
        auto current_calories = elve_calories[elve];

        if(current_calories > std::get<1>(elve_1)) {
            std::get<0>(elve_3) = std::get<0>(elve_2);
            std::get<1>(elve_3) = std::get<1>(elve_2);
            std::get<0>(elve_2) = std::get<0>(elve_1);
            std::get<1>(elve_2) = std::get<1>(elve_1);
            std::get<0>(elve_1) = elve + 1;
            std::get<1>(elve_1) = current_calories;
        } else if(current_calories > std::get<1>(elve_2) and current_calories != std::get<1>(elve_1)) {
            std::get<0>(elve_3) = std::get<0>(elve_2);
            std::get<1>(elve_3) = std::get<1>(elve_2);
            std::get<0>(elve_2) = elve + 1;
            std::get<1>(elve_2) = current_calories;
        } else if(current_calories > std::get<1>(elve_3) and current_calories != std::get<1>(elve_2)) {
            std::get<0>(elve_3) = elve + 1;
            std::get<1>(elve_3) = current_calories;
        }
    }

    std::cout << std::get<0>(elve_1) << " " << std::get<1>(elve_1) << "\n";
    std::cout << std::get<0>(elve_2) << " " << std::get<1>(elve_2) << "\n";
    std::cout << std::get<0>(elve_3) << " " << std::get<1>(elve_3) << "\n";

   // The easy route:
   // std::sort(elve_calories.begin(), elve_calories.end(), std::greater<long>());
   // std::cout << elve_calories[0] << std::endl;
   // std::cout << elve_calories[1] << std::endl;
   // std::cout << elve_calories[2] << std::endl;
}
