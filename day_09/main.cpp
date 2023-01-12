#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <ranges>
#include <algorithm>
#include <unordered_set>

struct command {
    enum class Direction {
        left,
        right,
        up,
        down
    };
    Direction direction;
    unsigned int count;
};

static std::map<char, command::Direction> direction_map = {
    {'L', command::Direction::left},
    {'R', command::Direction::right},
    {'U', command::Direction::up},
    {'D', command::Direction::down}
};

static void move(command const& command, int& X, int& Y) {
    switch(command.direction) {
        case command::Direction::left: X--;
        break;
        case command::Direction::right: X++;
        break;
        case command::Direction::up: Y--;
        break;
        case command::Direction::down: Y++;
        break;
    }
}

static void move_diagonal(int headX, int headY, int& tailX, int& tailY) {
    int diffX = headX - tailX;
    int diffY = headY - tailY;

    if(diffX < 0 and diffY < 0) {
        tailX--;
        tailY--;
    } else if(diffX > 0 and diffY < 0) {
        tailX++;
        tailY--;
    } else  if(diffX < 0 and diffY > 0) {
        tailX--;
        tailY++;
    } else if(diffX > 0 and diffY > 0) {
        tailX++;
        tailY++;
    }
}

static void part_one(std::vector<command> const& commands) {
    std::vector<std::pair<int, int>> already_visited{};

    // start position is not important
    int headY = 100;
    int headX = 100;

    int tailY = 100;
    int tailX = 100;

    unsigned int counter{0};

    auto was_already_visited = [&tailX, &tailY](std::pair<int, int> const& p){ return tailX == p.first and tailY == p.second; };

    for(auto const& command : commands) {
        for(int i = 0; i < command.count; i++) {
            move(command, headX, headY);

            bool should_move_row_or_column = (std::abs(tailX - headX) > 1 and tailY == headY) or (std::abs(tailY - headY) > 1 and tailX == headX);
            bool should_move_diagonal = std::abs(tailX - headX) > 1 or std::abs(tailY - headY) > 1;

            if(should_move_row_or_column) {
                move(command, tailX, tailY);
            } else if(should_move_diagonal) {
                move_diagonal(headX, headY, tailX, tailY);
            }

            // TODO map x Y coord to one coord e.g cantor pairing ?
            if(auto ptr = std::ranges::find_if(already_visited, was_already_visited); ptr == already_visited.end()) {
                counter++;
                already_visited.push_back(std::make_pair(tailX, tailY));
            }
        }
    }

    std::cout << counter << std::endl;
}

int main() {
    std::ifstream file("./input/input_day09.txt");

    if(file.fail()) return 1;

    std::string line;
    std::vector<command> commands{};

    while(std::getline(file, line)) {
        auto direction = direction_map.find(line[0])->second;
        unsigned int count = std::stoul(line.substr(2));

        commands.push_back({ .direction = direction, .count = count});
    }

    part_one(commands);
}
