#include <duels/snake/snake_ai.h>
#include <math.h>

using namespace duels::snake;
using duels::Result;

/************************ Level 0 IA *************************************/

Action SnakeAI::level0(){
    // Level 0 IA, just turning around the area without taking into account his environnement.
    switch (feedback.me.orientation) {
    case Orientation::UP : if(feedback.me.y-1 <0){return Action::TURN_LEFT;}else return Action::MOVE_FORWARD; break;
    case Orientation::DOWN : if(feedback.me.y+1>49){return Action::TURN_LEFT;}else return Action::MOVE_FORWARD; break;
    case Orientation::LEFT : if(feedback.me.x-1<0){return Action::TURN_LEFT;}else return Action::MOVE_FORWARD; break;
    case Orientation::RIGHT : if(feedback.me.x+1>69){return Action::TURN_LEFT;}else return Action::MOVE_FORWARD;break;
    }
}

/************************ Level 1 IA *************************************/
void SnakeAI::AddWallToObstacles(std::vector<Position> &obstacles)
{
    for(auto y = 1; y<50;y++)
    {
        obstacles.push_back({-1,y});
        obstacles.push_back({70,y});
    }
    for(auto x = 1; x<70; x++)
    {
        obstacles.push_back({x,-1});
        obstacles.push_back({x,50});
    }
}
void SnakeAI::AddOwnBodyToObstacles(const std::vector<Position> ownBody,std::vector<Position> &obstacles)
{
    for(auto position=ownBody.begin(); position!=ownBody.end(); position++)
    {
        obstacles.push_back({position->x,position->y});
    }

}

void SnakeAI::AllowedAction(const std::vector<Position> obstacles, const Position AfterTL, const Position AfterMF, const Position AfterTR,std::vector<Action> &Possibility )
{
bool testL=true, testF=true, testR = true;
int sizeObstacle=obstacles.size();

for (int obstacle = 0; obstacle<sizeObstacle; obstacle++)
{
 if(obstacles.at(obstacle).x==AfterTL.x)
 {
     if(obstacles.at(obstacle).y==AfterTL.y){testL=false;}
 }
 if(obstacles.at(obstacle).x==AfterMF.x)
 {
     if(obstacles.at(obstacle).y==AfterMF.y){testF=false;}
 }
 if(obstacles.at(obstacle).x==AfterTR.x)
 {
     if(obstacles.at(obstacle).y==AfterTR.y){testR=false;}
 }
}
 if (testL) {Possibility.push_back(Action::TURN_LEFT);}
 if (testF) {Possibility.push_back(Action::MOVE_FORWARD);}
 if (testR) {Possibility.push_back(Action::TURN_RIGHT);}

}

Action SnakeAI::NewAction(std::vector<Action> Possibility)
{   int size = Possibility.size();
    //std::default_random_engine enginePossibility;
    //std::uniform_int_distribution<> newAction(0, size);
    //int choice = newAction(enginePossibility);
    int choice = rand()%size;
    return Possibility.at(choice);

}
Action SnakeAI::level1()//move randomly without running into walls or itself
{
    Position l,f,r;
    std::vector<Position> obstacles;
    Position AfterTL, AfterMF, AfterTR;
    std::vector<Action> Possibility;

    switch (feedback.me.orientation) {
    case Orientation::UP : l={-1,0};f={0,-1};r={1,0}; break;
    case Orientation::RIGHT : l={0,-1};f={1,0};r={0,1}; break;
    case Orientation::DOWN : l={1,0};f={0,1};r={-1,0}; break;
    case Orientation::LEFT : l={0,1};f={-1,0};r={0,-1}; break;
    }

    AfterTL={feedback.me.x+l.x,feedback.me.y+l.y};
    AfterMF={feedback.me.x+f.x,feedback.me.y+f.y};
    AfterTR={feedback.me.x+r.x,feedback.me.y+r.y};

    AddWallToObstacles(obstacles);
    AddOwnBodyToObstacles(feedback.myBody,obstacles);

    AllowedAction(obstacles,AfterTL,AfterMF,AfterTR,Possibility);
    if (Possibility.size()>0)
    {

        return NewAction(Possibility);

    }
    return Action::MOVE_FORWARD;
}


/************************ Level 2 IA *************************************/



double SnakeAI::euclidianDistance(Position Cell1, Position Cell2){
    return sqrt(pow((Cell1.x - Cell2.x),2) + pow((Cell1.y - Cell2.y),2));
}

std::pair<int, double> SnakeAI::closestAppleIndex(std::vector<Position> &apples, Position &headPosition){
    Position closestApple = apples.at(0);
    int returnedIndex = 0;
    double closestDistance = euclidianDistance(headPosition, closestApple);
    for(auto apple = 0; apple < apples.size(); apple++)
    {
        if(euclidianDistance(headPosition, apples.at(apple)) < closestDistance)
        {
            returnedIndex = apple;
            closestApple = apples.at(returnedIndex);
            closestDistance = euclidianDistance(headPosition, closestApple);
        }
    }
    return {returnedIndex, closestDistance};
}


bool SnakeAI::checkFreeSpaceOnTurning(std::vector<Position> bodyMe, Pose headMe, std::vector<Position> body2, Pose head2, Position turningPos){
    bool free = true;

    // Checking first if the heads will collide to a border after turning left.
    if((headMe.x + turningPos.x < 0) or (headMe.x + turningPos.x > 69) or (headMe.y + turningPos.y < 0) or (headMe.y + turningPos.y > 49)){return false;}

    // Else, checking if there is an obstacle when turning left.
    std::vector<Position> obstacles = {};

    for(auto &part: bodyMe){obstacles.push_back(part);}
    obstacles.push_back({head2.x, head2.y});
    for(auto &part: body2){obstacles.push_back(part);}

    for(auto &obstacle: obstacles)
    {
        if((headMe.x + turningPos.x == obstacle.x) and (headMe.y + turningPos.y == obstacle.y)){free = false;}
    }

    return free;
}

Action SnakeAI::randomAction()
{
    int number = rand()%50;
    switch (number%3) {
    case 0: return Action::MOVE_FORWARD; break;
    case 1: return Action::TURN_LEFT; break;
    case 2: return Action::TURN_RIGHT; break;
    }
}

Action SnakeAI::level2(){ // Start looking for the closest apple and try to get there.

    // We first find the closest apple.
    Position headPosition = {feedback.me.x, feedback.me.y};
    std::pair<int, double> closestAppleInfo = closestAppleIndex(feedback.apples, headPosition); // contains the index of the closest apple and the distance to it.

    /* Then we decide the action we nee to ge there. We proceedd according to the following algorithm
     * moveForward if we get closer.
     * Otherwise, turnLeft, have to pay attention to his own body though.
     */
    Action returnedInput = Action::MOVE_FORWARD;
    switch (feedback.me.orientation) {
    case Orientation::UP :
        if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, {0, -1}) && feedback.me.y-1 >= 0 && (euclidianDistance({headPosition.x,headPosition.y-1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)){return returnedInput;}
        else {
            Position turnLeft = {-1, 0};
            Position turnRight = {+1, 0};
            if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnLeft)
                    && euclidianDistance({headPosition.x-1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)
            {returnedInput = Action::TURN_LEFT;}
            else if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnRight)
                    && euclidianDistance({headPosition.x+1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)
            {returnedInput = Action::TURN_RIGHT;}
            else{returnedInput = randomAction();}
        }
        break;
    case Orientation::DOWN :
        if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, {0, 1}) && feedback.me.y+1<=49 && (euclidianDistance({headPosition.x,headPosition.y+1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)){break;}
        else {
            Position turnLeft = {1, 0};
            Position turnRight = {-1, 0};
            if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnLeft)
                    && euclidianDistance({headPosition.x+1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){returnedInput = Action::TURN_LEFT;}
            else if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnRight)
                    && euclidianDistance({headPosition.x-1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)
            {returnedInput = Action::TURN_RIGHT;}
            else{returnedInput = randomAction();}
        }
        break;
    case Orientation::LEFT :
        if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, {-1, 0}) && feedback.me.x-1>= 0 && (euclidianDistance({headPosition.x-1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)){break;}
        else {
            Position turnLeft = {0, 1};
            Position turnRight = {0, -1};
            if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnLeft)
                    && euclidianDistance({headPosition.x,headPosition.y+1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){returnedInput = Action::TURN_LEFT;}
            else if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnRight)
                    && euclidianDistance({headPosition.x,headPosition.y-1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)
            {returnedInput = Action::TURN_RIGHT;}
            else{returnedInput = randomAction();}
        }
        break;
    case Orientation::RIGHT :
        if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, {1, 0}) && feedback.me.x+1<=69 && (euclidianDistance({headPosition.x+1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)){break;}
        else {
            Position turnLeft = {0, -1};
            Position turnRight = {0, 1};
            if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnLeft)
                    && euclidianDistance({headPosition.x,headPosition.y-1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){returnedInput = Action::TURN_LEFT;}
            else if(checkFreeSpaceOnTurning(feedback.myBody, feedback.me, feedback.opponentBody, feedback.opponent, turnRight)
                    && euclidianDistance({headPosition.x,headPosition.y+1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second)
            {returnedInput = Action::TURN_RIGHT;}
            else{returnedInput = randomAction();}
        }
        break;
    }

    return returnedInput;
}


void SnakeAI::updateInput()
{
    switch (difficulty) {
    case 0: input.action = level0(); break;
    case 1: input.action = level1(); break;
    case 2: input.action = level2(); break;
    }


    // in this function the `feedback` member variable was updated from the game
    // TODO update the `input` member variable
    // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
    // do not hesitate to create a .cpp file if this function is long

}



