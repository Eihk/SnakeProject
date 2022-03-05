#include <duels/snake/game.h>

using namespace duels::snake;

int main(int argc, char** argv)
{
  const std::string my_name = "your name";

  Game game(argc, argv, my_name, 1);    // to play as player 1 against level 1 AI
  //Game game(argc, argv, my_name, -2);    // to play as player 2 against level 2 AI

  Input input;
  Feedback feedback;
  [[maybe_unused]] const auto timeout = game.timeout_ms();

  while(game.get(feedback))
  {
    // write input in less than timeout



    game.send(input);
  }
}
