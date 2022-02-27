#include <duels/snake/mechanics.h>
#include <duels/utils/rand_utils.h>

/* ========== PUBLIC ==========*/

InitDisplay SnakeMechanics::initGame()
{
    InitDisplay msg;
    // Here are the parameters of the grid and the game state.
    //The dimension of the window.
    msg.height = height = 45;
    msg.width = width = 32;

    apples = {}; // The apples will be added during the initialisation
    // The snakes will always start with their orientation up.
    Orientation initOrientation = Orientation::UP;
    snake1.head.orientation = initOrientation;
    snake2.head.orientation = initOrientation;

    /* The snakes will always start at the same location.
     * Also, they will start with a boy of 5*/

    snake1.head.x = 6; snake1.head.y = 14;
    snake2.head.x = 39; snake2.head.y = 14;
    snake1.body = {{6,15},{6,16},{6,17},{6,18}};
    snake2.body = {{39,15},{39,16},{39,17},{39,18}};
}

void SnakeMechanics::newApple(std::vector<Position> &apples)
{
    static std::default_random_engine engine;
    std::uniform_int_distribution<> row(1, gameState.width-1);
    std::uniform_int_distribution<> column(1, gameState.height-1);

    return apples.push_back({row(engine), column(engine)}); 
}


bool SnakeMechanics::eatApple(Snake &snake, std::vector<Position> &apples)
{
    // Find which apple has been eaten first.
    for(int apple=0; apple < apples.size(); apple++)
    {
        if((snake.head.x == apples.at(apple).x) && (snake.head.y == apples.at(apple).y))
        {
            apples.erase(apples.begin() + apple);
            SnakeMechanics::newApple(apples);
            return true;
        }
    }
    SnakeMechanics::newApple(apples);
    return false;

}


void SnakeMechanics::turnLeft(Snake &snake)
{
    switch (snake.head.orientation) {
    case Orientation::UP : snake.head.orientation = Orientation::LEFT;
    case Orientation::LEFT : snake.head.orientation = Orientation::DOWN;
    case Orientation::DOWN : snake.head.orientation = Orientation::RIGHT;
    case Orientation::RIGHT : snake.head.orientation = Orientation::UP;
    }
    MovementBody(snake);
    MovementHead(snake);
}

void SnakeMechanics::turnRight(Snake &snake)
{
    switch (snake.head.orientation) {
    case Orientation::UP : snake.head.orientation = Orientation::RIGHT;
    case Orientation::RIGHT : snake.head.orientation = Orientation::DOWN;
    case Orientation::DOWN : snake.head.orientation = Orientation::LEFT;
    case Orientation::LEFT : snake.head.orientation = Orientation::UP;
    }
    MovementBody(snake);
    MovementHead(snake);
}

void SnakeMechanics::moveForward(Snake &snake)
{
    MovementBody(snake);
    MovementHead(snake);

}
void SnakeMechanics::MovementHead(Snake &snake)
{
    // It depends on the orientation of the snake.
    Orientation thisOne = snake.head.orientation;
    if(thisOne == Orientation::UP)    {snake.head.y += 1;}
    else if(thisOne == Orientation::LEFT)    {snake.head.x -= 1;}
    else if(thisOne == Orientation::DOWN)    {snake.head.y -= 1;}
    else{snake.head.x += 1;}
}
void SnakeMechanics::MovementBody(Snake &snake)
{//Check if the snake eat an apple and if it's the case increase it length
    int sizeSnake=snake.body.size();
    if (eatApple(snake,gameState.apples))
    {
        snake.body.push_back({snake.body[sizeSnake-1].x,snake.body[sizeSnake-1].y});
    }
    for(int bodypart=sizeSnake-1; bodypart >0; bodypart--)
    {
        snake.body[bodypart].x=snake.body[bodypart-1].x;
        snake.body[bodypart].y=snake.body[bodypart-1].y;
    }
    snake.body[0].x=snake.head.x;
    snake.body[0].y=snake.head.y;

}



void SnakeMechanics::update(const Input &input1, const Input &input2)
{
    switch(input1.action){
    case Action::MOVE_FORWARD : moveForward(gameState.snake1);
    case Action::TURN_LEFT : turnLeft(gameState.snake1);
    case Action::TURN_RIGHT : turnRight(gameState.snake1);
    }
    switch(input2.action){
    case Action::MOVE_FORWARD : moveForward(gameState.snake2);
    case Action::TURN_LEFT : turnLeft(gameState.snake2);
    case Action::TURN_RIGHT : turnRight(gameState.snake2);
    }

}

Result SnakeMechanics::buildPlayerFeedbacks(Feedback &feedback1, Feedback &feedback2)
{
    /* ========== One snake loses if his head hits the border or the other snake. =========== */
    // for the result, we will only use the feedback of one of the player as it gives the sames information.

    return Result::NONE; // To remove later on
    // Player 1
    feedback1.apples = gameState.apples;
    feedback1.me = gameState.snake1.head;
    feedback1.myBody = gameState.snake1.body;
    feedback1.opponent = gameState.snake2.head;
    feedback2.opponentBody = gameState.snake2.body;
    // Player 2
    feedback2.apples = gameState.apples;
    feedback2.me = gameState.snake2.head;
    feedback2.myBody = gameState.snake2.body;
    feedback2.opponent = gameState.snake1.head;
    feedback2.opponentBody = gameState.snake1.body;

    // It's a draw if both snakes collide to something, borders or other snake's body.
    if (((collideBorder(feedback1.me, xBorder, yBorder) || collideSnake({feedback1.me.x, feedback1.me.y}, feedback1.myBody, feedback1.opponentBody))
         && (collideBorder(feedback1.opponent, xBorder, yBorder) || collideSnake({feedback1.opponent.x, feedback1.opponent.y}, feedback1.opponentBody, feedback1.myBody))))
    {return Result::DRAW;}
    // P1 wins if P2 hits a border and P1 doesn't OR if P1 hits P2's body and P2 doesn't and vice versa
    else if ((collideBorder(feedback1.me, xBorder, yBorder) || collideSnake({feedback1.me.x, feedback1.me.y}, feedback1.myBody, feedback1.opponentBody))
              && !(collideBorder(feedback1.opponent, xBorder, yBorder) || collideSnake({feedback1.opponent.x, feedback1.opponent.y}, feedback1.opponentBody, feedback1.myBody)))
    {return Result::P2_WINS;}
    else if (!(collideBorder(feedback1.me, xBorder, yBorder) || collideSnake({feedback1.me.x, feedback1.me.y}, feedback1.myBody, feedback1.opponentBody))
             && !(collideBorder(feedback1.opponent, xBorder, yBorder) || collideSnake({feedback1.opponent.x, feedback1.opponent.y}, feedback1.opponentBody, feedback1.myBody)))
    {return Result::P1_WINS;}

//TODO
        return Result::NONE;}     // game goes on
}




/* ========== PRIVATE ==========*/

bool SnakeMechanics::collideSnake(const Position &element, const std::vector<Position> &me, const std::vector<Position> &opponent) // me or the oppoent doesn't matter, need to change the name.
{return((std::find(me.begin(), me.end(), element) != me.end()) || (std::find(opponent.begin(), opponent.end(), element) != opponent.end()));}

bool SnakeMechanics::collideBorder(const Pose &head, const int xBorder, const int yBorder)
{return (head.x == (0 || xBorder) || head.y == (0 || yBorder));}
