#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

// Set a bit for every character Z-A a-z
static std::bitset<52> create_character_index(std::string const &line) {
  static const unsigned int a_ascii_value = 97;
  static const unsigned int A_ascii_value = 65;
  static const std::bitset<52> one{0x1};

  std::bitset<52> character_index;

  for (auto const &c : line) {
    unsigned char priority = static_cast<unsigned char>(c);
    auto shift = std::isupper(priority) ? priority + 27 - A_ascii_value
                                        : priority + 1 - a_ascii_value;
    character_index |= (one << shift - 1);
  }

  return character_index;
}

static unsigned int part_one(std::string const &line) {
  unsigned int total_item_priorities{0};

  auto index1 = create_character_index(line.substr(0, line.size() / 2));
  auto index2 = create_character_index(line.substr(line.size() / 2));

  auto common_item = index1 & index2;

  for (std::size_t index = 0; index < 52; index++) {
    if (common_item[index]) {
      total_item_priorities = index + 1;
      break;
    }
  }

  return total_item_priorities;
}

static unsigned int part_two(std::string const (&lines)[3]) {
  unsigned int total_item_priorities{0};
  auto index1 = create_character_index(lines[0]);
  auto index2 = create_character_index(lines[1]);
  auto index3 = create_character_index(lines[2]);

  // and every index to get the common character (item)
  auto common_item = index1 & index2 & index3; 

  for (std::size_t index = 0; index < 52; index++) {
    if (common_item[index]) {
      total_item_priorities = index + 1;
      break;
    }
  }

  return total_item_priorities;
}

int main() {
  std::string lines[3];
  std::string line;
  std::ifstream file("./input/input_day03.txt");

  unsigned int total_item_priorities_part_one{0};
  unsigned int total_item_priorities_part_two{0};
  unsigned int counter{0};

  if (file.fail())
    return 1;

  while (std::getline(file, line)) {
    lines[counter++] = line;
    if (counter >= 3) {
      counter = 0;
      total_item_priorities_part_two += part_two(lines);
    }
    total_item_priorities_part_one += part_one(line);
  }

  std::cout << "Part one: " << total_item_priorities_part_one << std::endl;
  std::cout << "Part two: " << total_item_priorities_part_two << std::endl;
}
