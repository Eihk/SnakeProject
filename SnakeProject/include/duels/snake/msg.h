// generated from snake.yaml -- editing this file by hand is not recommended
#ifndef SNAKE_MSG_H
#define SNAKE_MSG_H
#include <sstream>
#include <duels/game_state.h>
namespace duels {
namespace snake {

// utility structures
enum class Orientation{LEFT,UP,RIGHT,DOWN};
struct Position
{
  int x;int y;
  inline bool operator==(const Position &other) const
  {
    return x == other.x && y == other.y;
  }
};
struct Pose
{
  int x;int y;Orientation orientation;
  inline bool operator==(const Pose &other) const
  {
    return x == other.x && y == other.y && orientation == other.orientation;
  }
};
enum class Action{TURN_LEFT,TURN_RIGHT,MOVE_FORWARD};
}}

//detail on how to stream these structures
#include "msg_detail.h"

// core game messages
namespace duels {
namespace snake {

struct InitDisplay
{
  std::vector<Position> snake1; std::vector<Position> snake2; int height; int width; std::vector<Position> apples; Pose head1; Pose head2;
  std::string serialize(std::string name1, std::string name2) const 
  {
    std::stringstream ss;
    ss << "name1: " << name1;
    ss << "\nname2: " << name2;
    ss << "\nsnake1: " << snake1;
    ss << "\nsnake2: " << snake2;
    ss << "\nheight: " << height;
    ss << "\nwidth: " << width;
    ss << "\napples: " << apples;
    ss << "\nhead1: " << head1;
    ss << "\nhead2: " << head2;
    return ss.str();
  }
};

struct Input
{
  Action action;
  std::string serialize() const 
  {
    std::stringstream ss;
    ss << "action: " << action;
    return ss.str();
  }
  void deserialize(const std::string &yaml)
  {
    const auto node{YAML::Load(yaml)};
    action = node["action"].as<Action>();
  }
};

struct Feedback
{
  Pose me; Pose opponent; std::vector<Position> apples; std::vector<Position> myBody; std::vector<Position> opponentBody; State __state;
  std::string serialize() const 
  {
    std::stringstream ss;
    ss << "me: " << me;
    ss << "\nopponent: " << opponent;
    ss << "\napples: " << apples;
    ss << "\nmyBody: " << myBody;
    ss << "\nopponentBody: " << opponentBody;
    ss << "\n__state: " << __state;
    return ss.str();
  }
  void deserialize(const std::string &yaml)
  {
    const auto node{YAML::Load(yaml)};
    me = node["me"].as<Pose>();
    opponent = node["opponent"].as<Pose>();
    apples = node["apples"].as<std::vector<Position>>();
    myBody = node["myBody"].as<std::vector<Position>>();
    opponentBody = node["opponentBody"].as<std::vector<Position>>();
    __state = node["__state"].as<State>();
  }
};

struct Display
{
  std::vector<Position> apples; Pose head1; Pose head2; std::vector<Position> body1; std::vector<Position> body2;
  std::string serialize(Result result) const 
  {
    std::stringstream ss;
    ss << "result: " << result;
    ss << "\napples: " << apples;
    ss << "\nhead1: " << head1;
    ss << "\nhead2: " << head2;
    ss << "\nbody1: " << body1;
    ss << "\nbody2: " << body2;
    return ss.str();
  }
};

}}
#endif
