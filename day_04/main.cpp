#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using number_pair = std::tuple<unsigned int, unsigned int>;
using section_pair = std::tuple<number_pair, number_pair>;

// Format <number>-<number>,<number>-<number>
static section_pair get_section_pair(std::string const& line) {
  auto comma_pos = line.find_first_of(',');
  auto first_seperator_pos = line.find_first_of('-');
  auto second_seperator_pos = line.find_last_of('-');

  auto first_number = line.substr(0, first_seperator_pos);
  auto second_number = line.substr(first_seperator_pos + 1, comma_pos - first_seperator_pos - 1);
  auto third_number = line.substr(comma_pos + 1, second_seperator_pos - comma_pos - 1);
  auto fourth_number = line.substr(second_seperator_pos + 1, line.size());

  number_pair pair_one = std::make_tuple(std::stoul(first_number), std::stoul(second_number));
  number_pair pair_two = std::make_tuple(std::stoul(third_number), std::stoul(fourth_number));

  return std::make_tuple(pair_one, pair_two);
}

static bool is_pair_fully_contained(number_pair const &pair_one, number_pair const &pair_two) {
  return (std::get<0>(pair_one) >= std::get<0>(pair_two) and std::get<1>(pair_one) <= std::get<1>(pair_two)) or
         (std::get<0>(pair_two) >= std::get<0>(pair_one) and std::get<1>(pair_two) <= std::get<1>(pair_one));
}

static bool is_overlapping(number_pair const &pair_one, number_pair const &pair_two) {
    auto s1n1 = std::get<0>(pair_one);
    auto s1n2 = std::get<1>(pair_one);
    auto s2n1 = std::get<0>(pair_two);
    auto s2n2 = std::get<1>(pair_two);

    return (s1n1 <= s2n2 and s2n1 <= s1n2) or is_pair_fully_contained(pair_one, pair_two);
}

int main() {
  std::string line;
  std::ifstream file("./input/input_day04.txt");

  if (file.fail())
    return 1;

  std::vector<section_pair> section_pairs{};

  while (std::getline(file, line)) {
    section_pairs.push_back(get_section_pair(line));
  }

  std::size_t part_one_counter{0};
  std::size_t part_two_counter{0};

  for (auto const &pairs : section_pairs) {
    if (is_pair_fully_contained(std::get<0>(pairs), std::get<1>(pairs))) part_one_counter++;
    if (is_overlapping(std::get<0>(pairs), std::get<1>(pairs))) part_two_counter++;
  }

  std::cout << "Part one: " << part_one_counter << std::endl;
  std::cout << "Part two: " << part_two_counter << std::endl;
}
