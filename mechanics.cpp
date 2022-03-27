#include <duels/snake/mechanics.h>
#include <duels/utils/rand_utils.h>

/* ========== PUBLIC ==========*/

InitDisplay SnakeMechanics::initGame()
{
    InitDisplay msg;
    // Here are the parameters of the grid and the game state.
    //The dimension of the window.
    // 1120*800
    msg.height = height = 50;
    msg.width = width = 70;
    apples = {};
    while(apples.size() < 3)
    {newApple(apples);}
    Pose SnakeHead;
    std::vector<Position> SnakeBody = {};
    InitSnake(SnakeHead,SnakeBody);
    snake1.head=SnakeHead; snake1.body=SnakeBody;
    SnakeBody={};
    InitSnake(SnakeHead,SnakeBody);
    snake2.head=SnakeHead; snake2.body=SnakeBody;


    msg.head1 = snake1.head;
    msg.head2 = snake2.head;
    msg.body1 = snake1.body;
    msg.body2 = snake2.body;
    return msg;
}

bool SnakeMechanics::PosPossible(const Position PosToTest,const Pose SnakeHead,const std::vector<Position> snakeBody)
{
    bool test = true;
    if (PosToTest.x < 0 || PosToTest.x >= width || PosToTest.y<0 || PosToTest.y >= height)
    {test=false;}
    if ((PosToTest.x==SnakeHead.x && PosToTest.y==SnakeHead.y)||FindinList(PosToTest,snakeBody))
    {test=false;}
    if ((PosToTest.x==snake1.head.x && PosToTest.y==snake1.head.y)||FindinList(PosToTest,snake1.body))
    {test=false;}
    return test;
}

void SnakeMechanics::InitSnake(Pose &snakeHead , std::vector<Position> &snakeBody)
{   static std::default_random_engine EngineSnakeInit;
    std::uniform_int_distribution<> orientation(0, 3);

    int snakeSizeIni=5;    //size of the snakes (head + body)
    int xmin,xmax,ymin,ymax,dx,dy;
    std::vector<Position> possibility;
    Position currentPart;

    int initOrientation = orientation(EngineSnakeInit);
    switch(initOrientation){
        case 0: snakeHead.orientation=Orientation::UP; xmin=ymin=0;xmax=width-1; ymax = height-2; dx=0;dy=1;
        case 1: snakeHead.orientation=Orientation::RIGHT; xmin=1;ymin=0;xmax=width-1; ymax = height-1; dx=-1;dy=0;
        case 2: snakeHead.orientation=Orientation::DOWN; xmin=0;ymin=1;xmax=width-1; ymax = height-1; dx=0;dy=-1;
        case 3: snakeHead.orientation=Orientation::LEFT; xmin=ymin=0;xmax=width-2; ymax = height-1; dx=1;dy=0;
    }

    std::uniform_int_distribution<> row(xmin, xmax);
    std::uniform_int_distribution<> column(ymin,ymax);
    snakeHead.x=row(EngineSnakeInit);
    snakeHead.y=column(EngineSnakeInit);


    while (FindinList({snakeHead.x,snakeHead.y},snake1.body) || (snakeHead.x==snake1.head.x && snakeHead.y==snake1.head.y) ||           //Check if the head isn't on the first snake
           FindinList({snakeHead.x+dx,snakeHead.y+dy},snake1.body) || (snakeHead.x+dx==snake1.head.x && snakeHead.y+dy==snake1.head.y)) //Chech if the first element of the body isn't on the first snake
    {
        snakeHead.x=row(EngineSnakeInit);
        snakeHead.y=column(EngineSnakeInit);

    }

    currentPart={snakeHead.x+dx,snakeHead.y+dy};
    snakeBody.push_back(currentPart);


    while (snakeBody.size()<(snakeSizeIni-1))
    {
        possibility={};
        if (PosPossible({currentPart.x+1,currentPart.y},snakeHead, snakeBody))  {possibility.push_back({currentPart.x+1,currentPart.y});}
        if (PosPossible({currentPart.x-1,currentPart.y},snakeHead, snakeBody))  {possibility.push_back({currentPart.x-1,currentPart.y});}
        if (PosPossible({currentPart.x,currentPart.y+1},snakeHead, snakeBody))  {possibility.push_back({currentPart.x,currentPart.y+1});}
        if (PosPossible({currentPart.x,currentPart.y-1},snakeHead, snakeBody))  {possibility.push_back({currentPart.x,currentPart.y-1});}

        if (possibility.size()==0)          // No possibility of continuing the placement of the snake, so we try all again
        {
            snakeBody={};
            InitSnake(snakeHead,snakeBody);
        }
        else {
            std::uniform_int_distribution<> ind(0,possibility.size()-1);
            int indice = ind(EngineSnakeInit);
            currentPart=possibility.at(indice);
            snakeBody.push_back(currentPart);
        }
    }
}

void SnakeMechanics::buildDisplayInformation()
{
    display_msg.body1 = snake1.body;
    display_msg.body2 = snake2.body;
    display_msg.head1.x = snake1.head.x;
    display_msg.head1.y = snake1.head.y;
    display_msg.head1.orientation = snake1.head.orientation;
    display_msg.head2.x = snake2.head.x;
    display_msg.head2.y = snake2.head.y;
    display_msg.head2.orientation = snake2.head.orientation;
    display_msg.apples = apples;
}

void SnakeMechanics::randomPosition(Snake &snake)
{
    static std::default_random_engine enginePos;
    std::uniform_int_distribution<> row(1, width-1);
    std::uniform_int_distribution<> column(1, height-1);
    std::uniform_int_distribution<> orientation(0, 3);


}

void SnakeMechanics::newApple(std::vector<Position> &apples)
{
    static std::default_random_engine engineApple;
    std::uniform_int_distribution<> row(1, width-1);
    std::uniform_int_distribution<> column(1, height-1);

    std::vector<Position> snakeAreThere = {};

    snakeAreThere.push_back({snake1.head.x,snake1.head.y});
    for(auto &body: snake1.body){snakeAreThere.push_back(body);}
    snakeAreThere.push_back({snake2.head.x,snake2.head.y});
    for(auto &body: snake2.body){snakeAreThere.push_back(body);}

    Position newPos = {row(engineApple), column(engineApple)};
    int numberOfVeritication = 0;

    while(numberOfVeritication != size(snakeAreThere))
    {
        if(newPos == snakeAreThere.at(numberOfVeritication))
        {
            numberOfVeritication = 0;
            newPos = {row(engineApple), column(engineApple)};
        }
        else{numberOfVeritication++;}
    }

    return apples.push_back(newPos);
}


void SnakeMechanics::eatApple(Snake &snake, std::vector<Position> &apples)
{
    // Find which apple has been eaten first.
    int sizeSnake = snake.body.size();
    for(int apple=0; apple < apples.size(); apple++)
    {
        if((snake.head.x == apples.at(apple).x) && (snake.head.y == apples.at(apple).y))
        {
            apples.erase(apples.begin() + apple);
            SnakeMechanics::newApple(apples);
            snake.body.push_back({snake.body.at(sizeSnake-1)});
            for(int bodypart=sizeSnake; bodypart >0; bodypart--)
            {
                snake.body[bodypart].x=snake.body[bodypart-1].x;
                snake.body[bodypart].y=snake.body[bodypart-1].y;
            }
            snake.body[0].x=snake.head.x;
            snake.body[0].y=snake.head.y;
        }
    }
}

void SnakeMechanics::turnLeft(Snake &snake)
{
    switch (snake.head.orientation) {
    case Orientation::UP : snake.head.orientation = Orientation::LEFT; break;
    case Orientation::LEFT : snake.head.orientation = Orientation::DOWN; break;
    case Orientation::DOWN : snake.head.orientation = Orientation::RIGHT; break;
    case Orientation::RIGHT : snake.head.orientation = Orientation::UP; break;
    }
    moveForward(snake);
}

void SnakeMechanics::turnRight(Snake &snake)
{
    switch (snake.head.orientation) {
    case Orientation::UP : snake.head.orientation = Orientation::RIGHT; break;
    case Orientation::RIGHT : snake.head.orientation = Orientation::DOWN; break;
    case Orientation::DOWN : snake.head.orientation = Orientation::LEFT; break;
    case Orientation::LEFT : snake.head.orientation = Orientation::UP; break;
    }
    moveForward(snake);
}

void SnakeMechanics::moveForward(Snake &snake)
{// We first move the body then the head
    int sizeSnake=snake.body.size();
    for(auto bodyPart = sizeSnake-1 ; bodyPart > 0 ; bodyPart--)
    {
        snake.body.at(bodyPart) = snake.body.at(bodyPart-1);
    }
    snake.body.at(0).x = snake.head.x;
    snake.body.at(0).y = snake.head.y;

    Orientation thisOne = snake.head.orientation;
    if(thisOne == Orientation::UP)    {snake.head.y -= 1;}
    else if(thisOne == Orientation::LEFT)    {snake.head.x -= 1;}
    else if(thisOne == Orientation::DOWN)    {snake.head.y += 1;}
    else{snake.head.x += 1;}
}

void SnakeMechanics::update(const Input &input1, const Input &input2)
{
    eatApple(snake1, apples);
    eatApple(snake2, apples);
    switch(input1.action){
    case Action::MOVE_FORWARD : moveForward(snake1); break;
    case Action::TURN_LEFT : turnLeft(snake1); break;
    case Action::TURN_RIGHT : turnRight(snake1); break;
    }
    switch(input2.action){
    case Action::MOVE_FORWARD : moveForward(snake2); break;
    case Action::TURN_LEFT : turnLeft(snake2); break;
    case Action::TURN_RIGHT : turnRight(snake2);break;
    }

}

Result SnakeMechanics::buildPlayerFeedbacks(Feedback &feedback1, Feedback &feedback2)
{
    /* ========== One snake loses if his head hits the border or the other snake. =========== */
    // for the result, we will only use the feedback of one of the player as it gives the sames information.
    // Player 1
    feedback1.apples = apples;
    feedback1.me = snake1.head;
    feedback1.myBody = snake1.body;
    feedback1.opponent = snake2.head;
    feedback1.opponentBody = snake2.body;
    // Player 2
    feedback2.apples = apples;
    feedback2.me = snake2.head;
    feedback2.myBody = snake2.body;
    feedback2.opponent = snake1.head;
    feedback2.opponentBody = snake1.body;

    //return Result::NONE; // to be removed later

    // It's a draw if both snakes collide to something, borders or other snake's body.
    if (((collideBorder(feedback1.me, width, height) || collideSnake({feedback1.me.x, feedback1.me.y}, feedback1.myBody, feedback1.opponentBody))
         && (collideBorder(feedback1.opponent, width, height) || collideSnake({feedback1.opponent.x, feedback1.opponent.y}, feedback1.opponentBody, feedback1.myBody))))
    {   std::cout<<"Draw ! "<<std::endl;
        return Result::DRAW;}
    // P1 wins if P2 hits a border and P1 doesn't OR if P1 hits P2's body and P2 doesn't and vice versa
    else if ((collideBorder(feedback1.me, width, height) || collideSnake({feedback1.me.x, feedback1.me.y}, feedback1.myBody, feedback1.opponentBody)))

    {   std::cout<<"P2 win ! "<<std::endl;
        return Result::P2_WINS;}
    else if (collideBorder(feedback1.opponent, width, height) || collideSnake({feedback1.opponent.x, feedback1.opponent.y}, feedback1.opponentBody, feedback1.myBody))
    {   std::cout<<"P1 win ! "<<std::endl;
        return Result::P1_WINS;}

//TODO
     return Result::NONE;
}     // game goes on





/* ========== PRIVATE ==========*/
bool SnakeMechanics::FindinList(const Position &element, const std::vector<Position> List)
{
    bool test = false;
    for (int ind=0; ind<List.size();ind++)
    {
        if (List[ind].x==element.x){if(List[ind].y==element.y){test = true;}}}
    return test;
}
bool SnakeMechanics::collideSnake(const Position &element, const std::vector<Position> &me, const std::vector<Position> &opponent) // me or the oppoent doesn't matter, need to change the name.
{return((FindinList(element, me)) || (FindinList(element, opponent)));}

bool SnakeMechanics::collideBorder(const Pose &head, const int xBorder, const int yBorder)
{return (head.x == -1) || (head.x== xBorder) || (head.y == -1) || (head.y == yBorder);}
