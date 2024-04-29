#include "Potion.hpp"

Potion::Potion(std::string& abbreviation, std::string& name, Dice *dice, Effect effect)
{
  abbreviation_ = abbreviation;
  name_ = name;
  dice_ = dice;
  effect_ = effect;
}

Effect Potion::getEffect() const
{
  return effect_;
}

Potion::~Potion()
{
  delete dice_;
}