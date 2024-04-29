#include "Ammunition.hpp"
#include "../utility/Utils.cpp"
#include <map>

Ammunition::Ammunition(std::string& abbreviation, std::string& name, std::string& type)
{
  abbreviation_ = abbreviation;
  name_ = name;
  type_ = parseType(type);
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