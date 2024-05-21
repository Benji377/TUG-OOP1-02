//---------------------------------------------------------------------------------------------------------------------
//
// The Weapon class is a subclass of the Item class. It represents a weapon that can be used by the player. The weapon
// has a damage pattern, an attack type, a damage type and a damage addition.
// The damage pattern is a WeaponDamagePattern object that stores the affected fields by the attack. The attack type is
// either melee or ranged. The damage type is either physical, fire, cold, acid or force. The damage addition
// is added to the damage roll and depends on the characters Vitality and Strength.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"
#include "DamagePattern.hpp"


enum class AttackType
{
  MELEE, RANGED
};
enum class DamageType
{
  NONE, PHYSICAL, FIRE, COLD, ACID, FORCE
};


class Weapon : public Item
{
  std::shared_ptr<DamagePattern> damage_pattern_;
  AttackType attack_type_;
  DamageType damage_type_;
  int damage_addition_; // Gets added to the damage roll
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the attack type from a string to an enum.
  ///
  /// @param attack_type The attack type as a string.
  ///
  /// @return The attack type as an enum.
  //
  AttackType parseAttackType(std::string &attack_type);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the damage type from a string to an enum.
  ///
  /// @param damage_type The damage type as a string.
  ///
  /// @return The damage type as an enum.
  //
  DamageType parseDamageType(std::string &damage_type);

public:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// The constructor of the Weapon class. It initializes the abbreviation, name, dice, damage pattern, attack type,
  /// damage type and damage addition of the weapon.
  ///
  /// @param abbreviation The abbreviation of the weapon.
  /// @param name The name of the weapon.
  /// @param dice The dice of the weapon.
  /// @param damage_pattern The damage pattern of the weapon.
  /// @param attack_type The attack type of the weapon.
  /// @param damage_type The damage type of the weapon.
  /// @param damage_addition The damage addition of the weapon.
  //
  Weapon(std::string &abbreviation, std::string &name, const std::shared_ptr<Dice>& dice,
        const std::shared_ptr<DamagePattern>& damage_pattern,
         std::string &attack_type, std::string &damage_type, int damage_addition);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the damage addition of the weapon.
  ///
  /// @return The damage addition of the weapon.
  //
  int getDamageAddition() const { return damage_addition_; };

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the damage pattern of the weapon.
  ///
  /// @return The damage pattern of the weapon.
  //
  std::shared_ptr<DamagePattern> getDamagePattern() const { return damage_pattern_; };

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the attack type of the weapon.
  ///
  /// @return The attack type of the weapon.
  //
  AttackType getAttackType() const { return attack_type_; };

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the damage type of the weapon.
  ///
  /// @return The damage type of the weapon.
  //
  DamageType getDamageType() const { return damage_type_; };

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the damage of the weapon. Rolls the dice and then adds the damage addition to the result.
  ///
  /// @return The damage of the weapon.
  //
  int getDamage() const;

  //TODO is this alright? Necessary because these things change depending on character. If someone else picks it up it changes. -Hanno
  void setDamangePattern(std::shared_ptr<DamagePattern> damage_pattern) {damage_pattern_ = damage_pattern; };
  void setDamageType(DamageType type) {damage_type_ = type; };
  void setAttackType(AttackType type) {attack_type_ = type;};
  void setDamageAddition(int addition) {damage_addition_ = addition;};

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// The copy constructor of the Weapon class. It is deleted to prevent copying of Weapon objects.
  //
  Weapon(const Weapon &weapon) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// The assignment operator of the Weapon class. It is deleted to prevent copying of Weapon objects.
  //
  Weapon &operator=(const Weapon &weapon) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// The destructor of the Weapon class. It removes the Dice object of the weapon.
  //
  ~Weapon() = default;
};

#endif //WEAPON_HPP
