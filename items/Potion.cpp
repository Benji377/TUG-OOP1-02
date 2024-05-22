#include "Potion.hpp"

#include <utility>

Potion::Potion(std::string &abbreviation, std::string &name, std::string effect, std::shared_ptr<Dice> dice)
{
  abbreviation_ = abbreviation;
  name_ = name;
  dice_ = std::move(dice);
  effect_ = parseEffect(effect);
}

Effect Potion::parseEffect(std::string &effect)
{
  Utils::normalizeString(effect);
  // Uses the map function to map the string to the enum
  std::map<std::string, Effect> effect_map = {
          {"health", Effect::HEALTH},
          {"fire",   Effect::FIRE},
          {"cold",   Effect::COLD},
          {"force",  Effect::FORCE},
          {"acid",   Effect::ACID}
  };
  if (effect_map.count(effect) == 0)
  {
    throw std::invalid_argument("[POTION] Invalid effect");
  }
  return effect_map[effect];
}

Effect Potion::getEffect() const
{
  return effect_;
}