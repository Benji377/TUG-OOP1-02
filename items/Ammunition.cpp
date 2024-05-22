#include "Ammunition.hpp"

Ammunition::Ammunition(std::string &abbreviation, std::string &type, std::vector<std::string> &weapons, int amount)
{
  abbreviation_ = abbreviation;
  name_ = type;
  type_ = parseType(type);
  weapons_ = weapons;
  amount_ = amount;
}

AmmunitionType Ammunition::parseType(std::string &type)
{
  Utils::normalizeString(type);
  std::map<std::string, AmmunitionType> type_map = {
          {"arrow", AmmunitionType::ARROW},
          {"bolt",  AmmunitionType::BOLT}
  };
  if (type_map.count(type) == 0)
  {
    throw std::invalid_argument("[AMMUNITION] Invalid ammunition type.");
  }
  return type_map[type];
}

AmmunitionType Ammunition::getType() const
{
  return type_;
}

void Ammunition::setAmount(int amount)
{
  amount_ = amount;
}

int Ammunition::getAmount() const
{
  return amount_;
}
