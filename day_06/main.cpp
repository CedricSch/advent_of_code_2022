#include <fstream>
#include <iostream>
#include <string>

// naive approach
static bool has_uniqe_chars(std::string const& window) {
    for(std::size_t i{0}; i < window.size(); i++) {
        for(std::size_t j{i+1}; j < window.size(); j++) {
            if(window[i] == window[j]) return false;
        }
    }
    return true;
}

static int get_start_of_packet_marker_part_two(std::size_t start, std::string const& line) {
    // Slide the start + 1 window to the right one character at the the time.
    for(std::size_t i{start}; i < line.size(); i++) {
        if(has_uniqe_chars(line.substr(i-start, start+1))) {
            return i+1;
        }
    }
    
    return -1;
}

int main() {
  std::string line;
  std::ifstream file("input.txt");

  if (file.fail())
    return 1;

  while (std::getline(file, line)) {
      std::cout << "Part one marker: " << get_start_of_packet_marker_part_two(3, line) << "\n";
      std::cout << "Part two marker: " << get_start_of_packet_marker_part_two(13, line) << "\n";
  }
}
