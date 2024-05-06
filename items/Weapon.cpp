#include "Weapon.hpp"

Weapon::Weapon(std::string &abbreviation, std::string &name, Dice *dice, DamagePattern *damage_pattern,
               std::string &attack_type, std::string &damage_type, int damage_addition)
{
  abbreviation_ = abbreviation;
  name_ = name;
  dice_ = dice;
  damage_pattern_ = damage_pattern;
  damage_addition_ = damage_addition;
  attack_type_ = parseAttackType(attack_type);
  damage_type_ = parseDamageType(damage_type);
}

int Weapon::getDamageAddition() const
{
  return damage_addition_;
}

AttackType Weapon::getAttackType() const
{
  return attack_type_;
}

DamageType Weapon::getDamageType() const
{
  return damage_type_;
}

Weapon::~Weapon()
{
  delete dice_;
}

AttackType Weapon::parseAttackType(std::string &attack_type)
{
  Utils::normalizeString(attack_type);
  std::map<std::string, AttackType> attackTypeMap = {
          {"melee",  AttackType::MELEE},
          {"ranged", AttackType::RANGED}
  };
  if (attackTypeMap.count(attack_type) == 0)
  {
    throw std::invalid_argument("Invalid attack type");
  }
  return attackTypeMap[attack_type];
}

DamageType Weapon::parseDamageType(std::string &damage_type)
{
  Utils::normalizeString(damage_type);
  std::map<std::string, DamageType> damageTypeMap = {
          {"physical", DamageType::PHYSICAL},
          {"fire",     DamageType::FIRE},
          {"cold",     DamageType::COLD},
          {"acid",     DamageType::ACID},
          {"force",    DamageType::FORCE}
  };
  if (damageTypeMap.count(damage_type) == 0)
  {
    throw std::invalid_argument("Invalid damage type");
  }
  return damageTypeMap[damage_type];
}