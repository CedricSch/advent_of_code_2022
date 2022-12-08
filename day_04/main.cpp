#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

using number_pair = std::tuple<unsigned int, unsigned int>;
using section_pair = std::tuple<number_pair, number_pair>;

// Format <number>-<number>,<number>-<number>
// use stack to get the numbers
// static section_pair get_section_pair2(std::string const& line) {
//     // find , delimiter
//     auto sep = line.find_first_of(',');
//     std::string first_section_pair = line.substr(0, sep);
//     std::string second_section_pair = line.substr(sep+1, line.size());
//
//     return {};
// }

// Format <number>-<number>,<number>-<number>
static section_pair get_section_pair(std::string_view line) {
  auto comma_pos = line.find_first_of(',');
  auto first_seperator_pos = line.find_first_of('-');
  auto second_seperator_pos = line.find_last_of('-');

  auto first_number = line.substr(0, first_seperator_pos);
  auto second_number = line.substr(first_seperator_pos + 1, comma_pos - first_seperator_pos - 1);
  auto third_number = line.substr(comma_pos + 1, second_seperator_pos - comma_pos - 1);
  auto fourth_number = line.substr(second_seperator_pos + 1, line.size());

  number_pair pair_one = std::make_tuple(std::stoul(std::string(first_number)), std::stoul(std::string(second_number)));
  number_pair pair_two = std::make_tuple(std::stoul(std::string(third_number)), std::stoul(std::string(fourth_number)));

  return std::make_tuple(pair_one, pair_two);
}

static bool is_pair_fully_contained(number_pair const &pair_one, number_pair const &pair_two) {
  return std::get<0>(pair_one) >= std::get<0>(pair_two) and std::get<1>(pair_one) <= std::get<1>(pair_two) or
         std::get<0>(pair_two) >= std::get<0>(pair_one) and std::get<1>(pair_two) <= std::get<1>(pair_one);
}

static bool is_overlapping(number_pair const &pair_one, number_pair const &pair_two) {
    auto s1n1 = std::get<0>(pair_one);
    auto s1n2 = std::get<1>(pair_one);
    auto s2n1 = std::get<0>(pair_two);
    auto s2n2 = std::get<1>(pair_two);

    bool not_overlapping = (s1n2 < s2n1 and s1n1 < s2n2) or (s1n1 > s2n2 and s1n2 > s2n2);

    return not not_overlapping or is_pair_fully_contained(pair_one, pair_two);
}

int main() {
  std::string line;
  std::ifstream file("input.txt");

  if (file.fail())
    return 1;

  std::vector<section_pair> section_pairs{};

  while (std::getline(file, line)) {
    section_pairs.push_back(get_section_pair(line));
  }

  std::size_t counter1{0};
  std::size_t counter2{0};

  for (auto const &pairs : section_pairs) {
    if (is_pair_fully_contained(std::get<0>(pairs), std::get<1>(pairs))) counter1++;
    if (is_overlapping(std::get<0>(pairs), std::get<1>(pairs))) counter2++;
  }

  std::cout << "Part One: " << counter1 << "\n";
  std::cout << "Part Two: " << counter2 << std::endl;
}
