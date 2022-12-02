#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

enum class Result {
  WIN,
  DRAW,
  LOSS
};

enum class Shape {
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
};

/*
 * Opponent:
 * A = Rock, B = Paper, C = Scissors
 *
 * You:
 * X = Rock, Y = Paper, Z = Scissors
 *
 * - Rock defeats Scissors
 * - Scissors defeats Paper
 * - Paper defeats Rock
 *
 * */

unsigned int get_points(std::tuple<Shape, Shape> const& shapes) {
    auto opponent = std::get<0>(shapes);
    auto player = std::get<1>(shapes);

    static unsigned int const POINTS_WIN = 6;
    static unsigned int const POINTS_DRAW = 3;
    static unsigned int const POINTS_LOSS = 0;

    // if both the opponent and player have a draw (= same shape) return the shape value
    if(opponent == player) return POINTS_DRAW + static_cast<unsigned int>(opponent); 
    // loss
    else if((opponent == Shape::ROCK and player == Shape::SCISSORS) or 
       (opponent == Shape::SCISSORS and player == Shape::PAPER) or
       (opponent == Shape::PAPER and player == Shape::ROCK)) {
        return POINTS_LOSS + static_cast<unsigned int>(player);
    }
    else return POINTS_WIN + static_cast<unsigned int>(player);
}

int main() {
    std::vector<std::tuple<Shape, Shape>> strategy_guide{};

    std::string line;
    std::ifstream file("data.txt");

    if(file.fail()) return 1;
     
    // read strategy guide from file into vector
    while(std::getline(file, line)) {
        std::tuple<Shape, Shape> current_round{};
        if(line[0] == 'A') {
            std::get<0>(current_round) = Shape::ROCK;
        } else if(line[0] == 'B') {
            std::get<0>(current_round) = Shape::PAPER;
        } else if(line[0] == 'C') {
            std::get<0>(current_round) = Shape::SCISSORS;
        } 
        if(line[2] == 'X') {
            std::get<1>(current_round) = Shape::ROCK;
        } else if(line[2] == 'Y') {
            std::get<1>(current_round) = Shape::PAPER;
        } else if(line[2] == 'Z') {
            std::get<1>(current_round) = Shape::SCISSORS;
        } 
        // ofc this assumes valid input
        strategy_guide.push_back(current_round);
    }

    unsigned int total_score{0};

    for(auto const& shape : strategy_guide) {
      total_score += get_points(shape);
    }

    std::cout << total_score << std::endl;

}
