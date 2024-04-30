#include "Potion.hpp"
#include <map>
#include "../utility/Utils.cpp"

Potion::Potion(std::string& abbreviation, std::string& name, std::string& effect, Dice *dice)
{
  abbreviation_ = abbreviation;
  name_ = name;
  dice_ = dice;
  effect_ = parseEffect(effect);
}

Effect Potion::parseEffect(std::string& effect)
{
  Utils::normalizeString(effect);
  // Uses the map function to map the string to the enum
  std::map<std::string, Effect> effectMap = {
          {"health", Effect::HEALTH},
          {"fire", Effect::FIRE},
          {"cold", Effect::COLD},
          {"force", Effect::FORCE},
          {"acid", Effect::ACID}
  };
  if (effectMap.count(effect) == 0)
  {
    // TODO: Replace with custom exception
    throw std::invalid_argument("Invalid effect");
  }
  return effectMap[effect];
}


Effect Potion::getEffect() const
{
  return effect_;
}

Potion::~Potion()
{
  delete dice_;
}