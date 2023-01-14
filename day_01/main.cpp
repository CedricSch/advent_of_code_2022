#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "test.h"

static const unsigned int EXPECTED_VALUE_PART_ONE = 72'602;
static const unsigned int EXPECTED_VALUE_PART_TWO = 207'410;

int main() {
  std::vector<long> elve_calories{};
  long elve_calorie_counter{0};

  std::string line;
  std::ifstream file("./input/input_day01.txt");

  if (file.fail())
    return 1;

  while (std::getline(file, line)) {
    if (line.empty()) {
      // new elve in next line
      elve_calories.push_back(elve_calorie_counter);
      elve_calorie_counter = 0;
      continue;
    }
    elve_calorie_counter += std::stol(line);
  }

  // Get the three elves with the most calories
  std::sort(elve_calories.begin(), elve_calories.end(), std::greater<long>());

  // Test part one and part two
  equals(EXPECTED_VALUE_PART_ONE, elve_calories[0]);
  equals(EXPECTED_VALUE_PART_TWO, elve_calories[0] + elve_calories[1] + elve_calories[2]);
}
