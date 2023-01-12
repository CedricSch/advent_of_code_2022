#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

enum class Shape { ROCK = 1, PAPER = 2, SCISSORS = 3 };

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
static Shape get_loss_shape(Shape const &opponent) {
  if (opponent == Shape::ROCK)
    return Shape::SCISSORS;
  else if (opponent == Shape::PAPER)
    return Shape::ROCK;
  else
    return Shape::PAPER;
}

static Shape get_win_shape(Shape const &opponent) {
  if (opponent == Shape::ROCK)
    return Shape::PAPER;
  else if (opponent == Shape::PAPER)
    return Shape::SCISSORS;
  else
    return Shape::ROCK;
}

static void set_strategy_shape(std::tuple<Shape, Shape> &shapes) {
  auto &opponent = std::get<0>(shapes);
  // X = loss, Y = draw, Z = win
  auto &player = std::get<1>(shapes);

  if (player == Shape::ROCK)
    player = get_loss_shape(opponent);
  else if (player == Shape::PAPER)
    player = opponent;
  else
    player = get_win_shape(opponent);
}

static unsigned int get_points(std::tuple<Shape, Shape> const &shapes) {
  auto opponent = std::get<0>(shapes);
  auto player = std::get<1>(shapes);

  static unsigned int const POINTS_WIN = 6;
  static unsigned int const POINTS_DRAW = 3;
  static unsigned int const POINTS_LOSS = 0;

  // if both the opponent and player have a draw (= same shape) return the shape
  // value
  if (opponent == player)
    return POINTS_DRAW + static_cast<unsigned int>(opponent);
  // loss
  else if ((opponent == Shape::ROCK and player == Shape::SCISSORS) or
           (opponent == Shape::SCISSORS and player == Shape::PAPER) or
           (opponent == Shape::PAPER and player == Shape::ROCK)) {
    return POINTS_LOSS + static_cast<unsigned int>(player);
  } 

  return POINTS_WIN + static_cast<unsigned int>(player);
}

int main() {
  std::vector<std::tuple<Shape, Shape>> strategy_guide{};

  std::string line;
  std::ifstream file("./input/input_day02.txt");

  if (file.fail())
    return 1;

  while (std::getline(file, line)) {
    // ofc this assumes valid input
    std::tuple<Shape, Shape> current_round{};

    if (line[0] == 'A') {
      std::get<0>(current_round) = Shape::ROCK;
    } else if (line[0] == 'B') {
      std::get<0>(current_round) = Shape::PAPER;
    } else if (line[0] == 'C') {
      std::get<0>(current_round) = Shape::SCISSORS;
    }
    if (line[2] == 'X') {
      std::get<1>(current_round) = Shape::ROCK;
    } else if (line[2] == 'Y') {
      std::get<1>(current_round) = Shape::PAPER;
    } else if (line[2] == 'Z') {
      std::get<1>(current_round) = Shape::SCISSORS;
    }
    strategy_guide.push_back(current_round);
  }

  unsigned int total_score{0};

  for (auto &shape : strategy_guide) {
    set_strategy_shape(shape);
    total_score += get_points(shape);
  }

  std::cout << "Part two: " << total_score << std::endl;
}
