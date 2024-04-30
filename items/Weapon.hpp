

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"
#include "WeaponDamagePattern.hpp"

enum class AttackType {MELEE, RANGED};
enum class DamageType {PHYSICAL, FIRE, COLD, ACID, FORCE};


class Weapon: public Item
{
  WeaponDamagePattern damage_pattern_;
  AttackType attack_type_;
  DamageType damage_type_;
  int damage_addition_; // Gets added to the damage roll
  AttackType parseAttackType(std::string &attack_type);
  DamageType parseDamageType(std::string &damage_type);
  public:
    Weapon(std::string &abbreviation, std::string &name, Dice* dice, WeaponDamagePattern &damage_pattern,
           std::string &attack_type, std::string &damage_type, int damage_addition);
    [[nodiscard]] int getDamageAddition() const;
    Weapon(const Weapon& weapon) = delete;
    Weapon& operator=(const Weapon& weapon) = delete;
    ~Weapon();
};


#endif //WEAPON_HPP
