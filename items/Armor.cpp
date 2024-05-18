#include "Armor.hpp"

Armor::Armor(std::string &abbreviation, std::string &name, int armor_value)
{
  abbreviation_ = abbreviation;
  name_ = name;
  armor_value_ = armor_value;
}

int Armor::getArmorValue() const
{
  return armor_value_;
}

void Armor::setArmorValue(int armor_value)
{
  armor_value_ = armor_value;
}