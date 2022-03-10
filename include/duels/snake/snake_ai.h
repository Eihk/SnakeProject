#ifndef SNAKE_AI_H
#define SNAKE_AI_H

#include <duels/player.h>
#include <duels/snake/msg.h>

namespace duels {
namespace snake {

// built-in AI class, should be heavily adapted to your needs
class SnakeAI : public duels::Player<Input, Feedback>
{
public:
  SnakeAI(int difficulty = 1) : difficulty(difficulty) {}

  void updateInput();
    // in this function the `feedback` member variable was updated from the game
    // TODO update the `input` member variable
    // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
    // do not hesitate to create a .cpp file if this function is long

private:
  int difficulty = 1;
  Action level0();
  Action level1();
  float euclidianDistance(Position Cell1, Position Cell2);
  std::pair<int, float> closestAppleIndex(std::vector<Position> &apples, Position &headPosition);

};
}
}
#endif
