//---------------------------------------------------------------------------------------------------------------------
//
// 
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ATTACKEDFIELD_HPP
#define ATTACKEDFIELD_HPP

#include <string>
#include <iostream>

class AttackedField
{
  private:
    bool contains_character_;
    std::pair<int, int> position_;
    std::string character_name_;
    int lost_health_;
    int total_damage_;
    int resistance_modifier_;
    int armor_value_;
    bool is_dead_;

  public:
    AttackedField(std::pair<int, int> position) : contains_character_(false), position_(position), lost_health_(0),
      total_damage_(0), resistance_modifier_(0), armor_value_(0), is_dead_(false) {}

    std::pair<int, int> getPosition() const { return position_; };
    void setCharacter(std::string name, int lost_health, int total_damage, int resistance_modifier, int armor_value,
      bool is_dead);
    void setCharacterWithoutName(int lost_health, int total_damage, int resistance_modifier, int armor_value,
      bool is_dead);
    void setName(std::string name) { character_name_ = name; }
    std::string getName() const { return character_name_; }
    bool containsCharacter() const { return contains_character_; }
    bool isDead() const { return is_dead_; }
    friend std::ostream& operator<<(std::ostream& os, const AttackedField& attacked_field);
};

std::ostream& operator<<(std::ostream& os, const AttackedField& attacked_field);

#endif // ATTACKEDFIELD_HPP