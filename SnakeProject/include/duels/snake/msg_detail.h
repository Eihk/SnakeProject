// generated from snake.yaml -- editing this file by hand is not recommended
inline std::ostream& operator<<(std::ostream& ss, const duels::snake::Position &position)
{
  ss << "{";
  ss << "x: " << position.x << ',';
  ss << "y: " << position.y << "}";
  return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const duels::snake::Pose &pose)
{
  ss << "{";
  ss << "x: " << pose.x << ',';
  ss << "y: " << pose.y << ',';
  ss << "orientation: " << pose.orientation << "}";
  return ss;
}

namespace YAML
{
template<>
struct convert<duels::snake::Position> 
{
  static bool decode(Node const& node, duels::snake::Position & rhs)
  {
    rhs.x = node["x"].as<int>();
    rhs.y = node["y"].as<int>();
    return true;
  }
};

template<>
struct convert<duels::snake::Pose> 
{
  static bool decode(Node const& node, duels::snake::Pose & rhs)
  {
    rhs.x = node["x"].as<int>();
    rhs.y = node["y"].as<int>();
    rhs.orientation = node["orientation"].as<duels::snake::Orientation>();
    return true;
  }
};
}