#ifndef SNAKE_MECHANICS_H
#define SNAKE_MECHANICS_H

#include <duels/snake/msg.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace duels::snake;
using duels::Result;

struct Snake
{
public:
    Pose head;
    std::vector<Position> body;

private:
    // Nothing here.
};

// base mechanics class, should be heavily adapted to reflect the game rules
class SnakeMechanics
{
public:
    SnakeMechanics() {}
    InitDisplay initGame();
    inline const Display& display() const {
        return display_msg;}

    // game evolution can be put here, or just save the inputs for later when building the feedbacks
    void update(const Input &input1, const Input &input2);
    // Input is an Action containing : TURN_LEFT,TURN_RIGHT,MOVE_FORWARD


    // should return who has just won, if any. May also compute display
    Result buildPlayerFeedbacks(Feedback &feedback1, Feedback &feedback2); // Borders correspond to the width and the lenght of the game's window

    void buildDisplayInformation();
    void randomPosition(Snake &snake);
    void randomPositionGenerator();
    void newApple(std::vector<Position> &apples);
    void turnLeft(Snake &snake);
    void turnRight(Snake &snake);
    void moveForward(Snake &snake);
    void eatApple(Snake &snake, std::vector<Position> &apples);
    void SnakeMovement(Snake &snake);



private:
    Display display_msg;
    int height, width;
    Snake snake1, snake2;
    std::vector<Position> apples;
    bool FindinList(const Position &element, const std::vector<Position> List);
    bool collideSnake(const Position &element, const std::vector<Position> &me, const std::vector<Position> &opponent); // Snake can collide to the opponent's Body or his own Body.
    bool collideBorder(const Pose &head, const int xBorder, const int yBorder);
};






#endif 
