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

float SnakeAI::euclidianDistance(Position Cell1, Position Cell2){
    return sqrt(pow((Cell1.x - Cell2.x),2) + pow((Cell1.y - Cell2.y),2));
}

std::pair<int, float> SnakeAI::closestAppleIndex(std::vector<Position> &apples, Position &headPosition){
    Position closestApple = apples.at(0);
    int returnedIndex = 0;
    float closestDistance = euclidianDistance(headPosition, closestApple);
    for(auto apple = 1; apple < apples.size(); apple++)
    {
        if(euclidianDistance(headPosition, apples.at(apple)) < closestDistance)
        {
            closestApple = apples.at(apple);
            closestDistance = euclidianDistance(headPosition, apples.at(apple));
        }
    }
    return {returnedIndex, closestDistance};
}

bool checkHeadOrientedTowardBody(Position firstBodyPart){
    return true;
}

Action SnakeAI::level1(){ // Start looking for the closest apple and try to get there.

    // We first fin the closest apple.
    Position headPosition = {feedback.me.x, feedback.me.y};
    std::pair<int, float> closestAppleInfo = closestAppleIndex(feedback.apples, headPosition); // contains the index of the closest apple and the distance to it.

    if(headPosition == feedback.apples.at(closestAppleInfo.first)){
        return Action::MOVE_FORWARD;
    }

    /* Then we decide the action we nee to ge there. We proceedd according to the following algorithm
     * moveForward if we get closer.
     * Otherwise, turnLeft, have to pay attention to his own body though.
     */
    Action returnedInput = Action::MOVE_FORWARD;
    switch (feedback.me.orientation) {
    case Orientation::UP :
        if(feedback.me.y-1 >0 and euclidianDistance({headPosition.x,headPosition.y-1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){break;}
        else returnedInput = Action::TURN_LEFT;
        break;
    case Orientation::DOWN :
        if(feedback.me.y+1<49 and euclidianDistance({headPosition.x,headPosition.y+1}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){break;}
        else returnedInput = Action::TURN_LEFT;
        break;
    case Orientation::LEFT :
        if(feedback.me.x-1>0 and euclidianDistance({headPosition.x-1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){break;}
        else returnedInput = Action::TURN_LEFT;
        break;
    case Orientation::RIGHT :
        if(feedback.me.x+1<69 and euclidianDistance({headPosition.x+1,headPosition.y}, feedback.apples.at(closestAppleInfo.first))<closestAppleInfo.second){break;}
        else returnedInput = Action::TURN_LEFT;
        break;
    }

    return returnedInput;

}


void SnakeAI::updateInput()
{
    switch (difficulty) {
    case 0: input.action = level0(); break;
    case 1: input.action = level1(); break;
    }


    // in this function the `feedback` member variable was updated from the game
    // TODO update the `input` member variable
    // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
    // do not hesitate to create a .cpp file if this function is long

}



