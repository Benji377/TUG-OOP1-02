#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>

enum ActionType {
  MOVE,
  ATTACK,
  USE,
  LOOT
};

class Action {
  private:
    ActionType type_;
    std::pair<int, int> target_position_;
    int distance_;
    int player_id_;

  public:
    Action(ActionType type, std::pair<int, int> target_position, int player_id)
      : type_(type), target_position_(target_position), player_id_(player_id) {}

    // Make move action
    Action(std::pair<int, int> target_position, int distance, int player_id)
      : type_(MOVE), target_position_(target_position), distance_(distance), player_id_(player_id) {}

    ActionType getType() const { return type_; }
    std::pair<int, int> getTargetPosition() const { return target_position_; }
    int getPlayerId() const { return player_id_; }
};

#endif // ACTION_HPP