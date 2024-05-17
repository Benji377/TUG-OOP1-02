#include "Weapon.hpp"

Weapon::Weapon(std::string &abbreviation, std::string &name, const std::shared_ptr<Dice>& dice,
                const std::shared_ptr<DamagePattern>& damage_pattern,
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

std::shared_ptr<DamagePattern> Weapon::getDamagePattern() const
{
  return damage_pattern_;
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
{}

AttackType Weapon::parseAttackType(std::string &attack_type)
{
  Utils::normalizeString(attack_type);
  std::map<std::string, AttackType> attackTypeMap = {
          {"melee",  AttackType::MELEE},
          {"ranged", AttackType::RANGED}
  };
  if (attackTypeMap.count(attack_type) == 0)
  {
    throw std::invalid_argument("[WEAPON] Invalid attack type");
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
    throw std::invalid_argument("[WEAPON] Invalid damage type");
  }
  return damageTypeMap[damage_type];
}

int Weapon::getDamage() const
{
  return getDice()->roll() + getDamageAddition();
}