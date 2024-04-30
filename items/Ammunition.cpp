#include "Ammunition.hpp"

using namespace ItemSpace;

Ammunition::Ammunition(std::string& abbreviation, std::string& type, std::vector<std::string> &weapons)
{
  abbreviation_ = abbreviation;
  type_ = parseType(type);
  weapons_ = weapons;
}

AmmunitionType Ammunition::parseType(std::string& type)
{
  Utils::normalizeString(type);
  std::map<std::string, AmmunitionType> typeMap = {
          {"arrow", AmmunitionType::ARROW},
          {"bolt", AmmunitionType::BOLT}
  };
  if (typeMap.count(type) == 0)
  {
    // TODO: Replace with custom exception
    throw std::invalid_argument("Invalid ammunition type.");
  }
  return typeMap[type];
}

AmmunitionType Ammunition::getType() const
{
  return type_;
}