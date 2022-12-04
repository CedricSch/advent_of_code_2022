#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

// Set a bit for every character Z-A a-z
// 10000000000000000000000001 10000000000000000000000001 -> Z, A and z, a are
// set.
static std::bitset<52> create_character_index(std::string const &line) {
  static unsigned int z_ascii_value = 97;
  static unsigned int Z_ascii_value = 65;

  std::bitset<52> character_index;
  std::bitset<52> one{0x1};

  for (auto const &c : line) {
    unsigned char priority = static_cast<unsigned char>(c);
    auto shift = std::isupper(priority) ? priority + 27 - Z_ascii_value
                                        : priority + 1 - z_ascii_value;
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

  auto common_item = index1 & index2 &
                     index3; // & every index to get the common character (item)

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
  std::ifstream file("input.txt");

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

  std::cout << "[Part one] Sum of priorities: " << total_item_priorities_part_one << std::endl;
  std::cout << "[Part two] Sum of priorities: " << total_item_priorities_part_two << std::endl;
}
