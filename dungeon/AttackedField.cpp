#include "AttackedField.hpp"

void AttackedField::setCharacter(std::string name, int lost_health, int total_damage, int resistance_modifier,
  int armor_value, bool is_dead)
{
  contains_character_ = true;
  character_name_ = name;
  lost_health_ = lost_health;
  total_damage_ = total_damage;
  resistance_modifier_ = resistance_modifier;
  armor_value_ = armor_value;
  is_dead_ = is_dead;
}

void AttackedField::setCharacterWithoutName(int lost_health, int total_damage, int resistance_modifier, int armor_value,
  bool is_dead)
{
  contains_character_ = true;
  lost_health_ = lost_health;
  total_damage_ = total_damage;
  resistance_modifier_ = resistance_modifier;
  armor_value_ = armor_value;
  is_dead_ = is_dead;
}

std::ostream& operator<<(std::ostream& os, const AttackedField& attacked_field)
{
  os << attacked_field.character_name_ << " loses " << attacked_field.lost_health_ << " health (" <<
    attacked_field.total_damage_ << " * " << attacked_field.resistance_modifier_ << " % - " <<
    attacked_field.armor_value_ << ")." << std::endl;
  return os;
}