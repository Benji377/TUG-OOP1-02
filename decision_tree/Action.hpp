#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../dungeon/AttackedField.hpp"

enum ActionType {
  MOVE,
  ATTACK,
  USE,
  LOOT
};

class Action {
  private:
    ActionType type_;
    char player_abbreviation_;
    std::pair<int, int> target_position_;
    int distance_;
    std::string item_name_;
    std::vector<AttackedField> attacked_fields_;
    int score_;

  public:
    // Loot action
    Action(ActionType type, char player_abbreviation, std::pair<int, int> target_position)
      : type_(type), player_abbreviation_(player_abbreviation), target_position_(target_position)
      {
        distance_ = 0;
        score_ = 0;
      }
    // Use action
    Action(ActionType type, char player_abbreviation, std::string item_name)
      : type_(type), player_abbreviation_(player_abbreviation), item_name_(item_name)
      {
        distance_ = 0;
        score_ = 0;
      }
    // Move action
    Action(ActionType type, char player_abbreviation, std::pair<int, int> target_position, int distance)
      : type_(type), player_abbreviation_(player_abbreviation), target_position_(target_position), distance_(distance)
      {
        score_ = 0;
      }
    // Attack action
    Action(ActionType type, char player_abbreviation, std::pair<int, int> target_position, std::vector<AttackedField> attacked_fields)
      : type_(type), player_abbreviation_(player_abbreviation), target_position_(target_position), attacked_fields_(attacked_fields)
      {
        distance_ = 0;
        score_ = 0;
      }

    ActionType getType() const { return type_; }
    std::pair<int, int> getTargetPosition() const { return target_position_; }
    char getPlayerAbbreviation() const { return player_abbreviation_; }
    int getScore() const { return score_; }
    int getDistance() const { return distance_; }
    void setScore(int score) { score_ = score; }
    void addScore(int score) { score_ += score; }
    std::vector<AttackedField> getAttackedFields() const { return attacked_fields_; }

    std::string getCommand() const {
      std::string command;
      switch (type_) {
        case MOVE:
          command = "move " + std::string(1, player_abbreviation_) + " " + std::to_string(target_position_.first) + "," + std::to_string(target_position_.second);
          break;
        case ATTACK:
          command = "attack " + std::string(1, player_abbreviation_) + " " + std::to_string(target_position_.first) + "," + std::to_string(target_position_.second);
          break;
        case USE:
          command = "use " + std::string(1, player_abbreviation_) + " " + item_name_;
          break;
        case LOOT:
          command = "loot " + std::string(1, player_abbreviation_) + " " + std::to_string(target_position_.first) + "," + std::to_string(target_position_.second);
          break;
      }
      return command;
    }
};

#endif // ACTION_HPP