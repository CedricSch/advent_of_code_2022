#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <ostream>
#include <ranges>
#include <unordered_set>
#include <functional>
#include <limits>

static int const MAX_COST = std::numeric_limits<int>::max();
static int const ROWS = 41;
static int const COLS = 172;

struct location {
    int x{-1};
    int y{-1};
    int cost{0};

    bool operator==(location const& rhs) const { return x == rhs.x and y == rhs.y; }
    bool operator>(location const& rhs) const { return cost > rhs.cost; };
};

namespace std {
    template<> struct hash<location> {
        size_t operator()(location const& loc) const {
            std::size_t h1 = std::hash<int>{}(loc.x);
            std::size_t h2 = std::hash<int>{}(loc.y);
            return h1 ^ (h2 << 1);
        }
    };
};

static std::vector<location> get_neighbours(int currentX, int currentY, std::vector<std::string> const& height_map) {
    std::vector<location> neighbours{};

    int left = currentX - 1;
    int right = currentX + 1;
    int up = currentY - 1;
    int down = currentY + 1;

    char current_height = height_map[currentY][currentX];

    if(left >= 0 and height_map[currentY][left] <= (current_height + 1)) neighbours.push_back({.x = left, .y = currentY});
    if(up >= 0   and height_map[up][currentX]   <= (current_height + 1)) neighbours.push_back({.x = currentX, .y = up});

    if(right < height_map[0].size() and height_map[currentY][right]  <= (current_height + 1)) neighbours.push_back({.x = right, .y = currentY});
    if(down < height_map.size()     and height_map[down][currentX]   <= (current_height + 1)) neighbours.push_back({.x = currentX, .y = down});

    return neighbours;
}

static int part_one(location const& start,  location const& end, std::vector<std::string> const& height_map) {
    int costs[ROWS][COLS] = {};
    bool visited_locations[ROWS][COLS] = {};
    bool already_pushed[ROWS][COLS] = {};

    std::priority_queue<location, std::vector<location>, std::greater<location>> to_visit;
    to_visit.push(start);

    while(not to_visit.empty()) {
        auto current_location = to_visit.top();
        to_visit.pop();

        visited_locations[current_location.y][current_location.x] = true;

        auto possible_neighbours = get_neighbours(current_location.x, current_location.y, height_map);

        // find good candidate
        for( location loc : possible_neighbours) {
            if(visited_locations[loc.y][loc.x] or already_pushed[loc.y][loc.x]) continue;

            loc.cost = current_location.cost + 1;

            costs[loc.y][loc.x] = loc.cost;
            already_pushed[loc.y][loc.x] = true;
             
            to_visit.push(loc);
        }
    }

    return costs[end.y][end.x];
}

static int part_two(location const& end, std::vector<std::string> const& height_map) { 
    int min_cost = MAX_COST;

    // get all locations with elevation a
    for(int row{0}; row < height_map.size(); row++) {
        for(int col{0}; col < height_map[0].size(); col++) {
            if(height_map[row][col] == 'a') {
              auto result = part_one({ .x = col, .y = row, .cost = 0 }, end, height_map);
              if(result > 0) min_cost = std::min(result, min_cost);
            }
        }
    }

    return min_cost;
}

int main() {
    std::ifstream file("./input/input_day12.txt");

    if(file.fail()) return 1;

    std::vector<std::string> height_map{};

    std::string line;
    location start;
    location end;

    while(std::getline(file, line)) {
        height_map.push_back(line);

        if(std::size_t pos = line.find_first_of('S'); pos != std::string::npos) {
            start = {.x = pos, .y = height_map.size() - 1};
            height_map.back()[pos] = 'a';
        }

        if(std::size_t pos = line.find_first_of('E'); pos != std::string::npos) {
            end = {.x = pos, .y = height_map.size() - 1};
            height_map.back()[pos] = 'z';
        }
    }

    std::cout << "Part one: " << part_one(start, end, height_map) << std::endl;
    std::cout << "Part two: " << part_two(end, height_map) << std::endl;
}
