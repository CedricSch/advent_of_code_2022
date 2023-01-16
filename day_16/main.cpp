#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <ranges>
#include <algorithm>
#include <stack>
#include "test.h"
#include <limits>

struct valve {
    int flow_rate{0};
    std::string name{};
    std::vector<std::string> connected_valves{};
};

static unsigned int const MAX_UINT = std::numeric_limits<int>::max();
static std::unordered_map<std::string, valve> valves{};

static std::vector<std::string> get_connected_valves(std::string_view line) {
    std::vector<std::string> connected_valves{};
    std::string filtered_word{};

    std::ranges::copy(line | std::views::filter([](char ch){ return not std::isspace(ch);}), std::back_inserter(filtered_word));

    for(auto word : filtered_word | std::views::split(',')) {
        connected_valves.push_back(std::string(word.begin(), word.end()));
    }

    return connected_valves;
}

static std::unordered_map<std::string, int> get_distances(std::unordered_map<std::string, valve> const& valves, valve const& start_valve) {
    std::unordered_map<std::string, int> distances{};
    // init all distances to "inifinity"
    for(auto const& [name, v] : valves) {
        distances.insert({name, MAX_UINT});
    }
    distances[start_valve.name] = 0;

    std::unordered_set<std::string> visited_valves{};
    std::vector<valve> valves_to_visit;

    valves_to_visit.push_back(valves.at(start_valve.name));

    while(not valves_to_visit.empty()) {
        auto current_valve = valves_to_visit.back();
        valves_to_visit.pop_back();

        visited_valves.insert(current_valve.name);

        for(std::string valve_name : current_valve.connected_valves) {
            if(visited_valves.contains(valve_name)) continue;

            distances[valve_name] = ((distances[current_valve.name] + 1) < distances[valve_name]) ? distances[current_valve.name] + 1 : distances[valve_name];
            valves_to_visit.push_back(valves.at(valve_name));
        }
    }

    std::erase_if(distances, [&valves](auto const& pair) { return valves.at(pair.first).flow_rate == 0 or pair.second == 0; });
    
    return distances;
}

static int dfs(std::unordered_map<std::string, std::unordered_map<std::string, int>> const& distances, valve const& current_valve, int time, std::set<std::string> open_valves) {
    int max_value = 0;

    for(auto [ other_valve, distance ] : distances.at(current_valve.name)) {
        if(open_valves.contains(other_valve)) continue;
        
        int remaining_time = time - distance - 1;

        if(remaining_time <= 0) continue;

        open_valves.insert(other_valve);
        max_value = std::max(max_value, dfs(distances, valves.at(other_valve), remaining_time, open_valves) + valves.at(other_valve).flow_rate * remaining_time);
    }

    return max_value;
}

static unsigned int part_one(std::unordered_map<std::string, valve> const& valves) {
    std::unordered_map<std::string, std::unordered_map<std::string, int>> valve_distances{};

    // calculate distances from every valve to every other valve
    for(auto const& [ name, valve ] : valves) {
        auto distances = get_distances(valves, valve);
        valve_distances[name] = distances;
    }

    for(auto const& [ a, b ] : valve_distances) {
        std::cout << a << " { ";
        for(auto [name, dist] : b) {
            std::cout << name << ": " << dist << "\t";
        }
        std::cout << " }\n";
    }

    return dfs(valve_distances, valves.at("AA"), 30, {});
}

// Traveling from one valve to another takes 1 minute
// Opening a vavle takes also 1 minute
int main() {
    std::ifstream file("./input/input_day16.txt");

    if(file.fail()) return 1;


    std::string line;
    while(std::getline(file, line)) {
        // get valve name
        auto ws_pos = line.find_first_of(' ');
        std::string valve_name = line.substr(ws_pos+1, 2);

        // get flow rate
        auto rate_pos = line.find_first_of('=');
        auto semi_pos = line.find_first_of(';');
        int flow_rate = std::stoi(line.substr(rate_pos+1, rate_pos+1-semi_pos));

        // get connected valves
        valve v{.flow_rate=flow_rate, .name = valve_name};
        if(auto pos = line.rfind("valves"); pos != std::string::npos) {
            v.connected_valves = get_connected_valves(line.substr(pos+6));
        } else if(auto pos = line.rfind("valve"); pos != std::string::npos) {
            v.connected_valves.push_back(line.substr(pos+6));
        }
        valves.insert(std::make_pair(valve_name, v));
    }

    auto result_part_one = part_one(valves);
    std::cout << result_part_one << std::endl;

    equals(1651, result_part_one);
}
