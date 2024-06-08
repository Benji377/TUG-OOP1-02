//---------------------------------------------------------------------------------------------------------------------
//
// The enemy class is derived from the Character class, and it represents an enemy in the game. It does change much
// from the Character class, but it is a separate class to make the code more readable and to make it easier to
// implement new features in the future. Its only notable difference is we check if its a boss.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class Enemy: public Character
{
  // If the enemy is a boss, aka Lich
  bool is_boss_;
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Initializes the Enemy inventory by creating ist weapons and loot. Gets called inside the constructor.
  //
  void initializeInventory();
  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Enemy class
    /// @param id the id of the enemy
    /// @param abbreviation the abbreviation of the enemy
    //
    Enemy(int id, char abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns if the enemy is a boss
    ///
    /// @return true if the enemy is a boss, false otherwise
    //
    bool isBoss() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the attack damage of the enemy. Depends on the equipped weapon and the dice roll.
    ///
    /// @return the attack damage of the enemy
    //
    int getAttackDamage() override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the attack damage of the enemy. This time also specifies the attack type.
    ///
    /// @param attack_type the attack type
    /// @return the attack damage of the enemy
    //
    int getAttackDamage(AttackType attack_type);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the remaining health after taking damage. The damage taken is influenced by Armor and Resistance
    ///
    /// @param damage the damage taken
    /// @param damage_type the type of the damage
    ///
    /// @return the remaining health
    //
    int takeDamage(int damage, DamageType damage_type) override;

    int simulateDamage(int damage, DamageType damage_type) override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the remaining health after taking damage. The damage taken is influenced by Armor and Resistance
    ///
    /// @param damage the damage taken
    /// @param damage_type the type of the damage
    ///
    /// @return the remaining health
    //
    void printEnemy(const std::string& id_string, const std::pair<int, int>& position) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the enemy's information
    //
    void simplePrint() const override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the Enemy attack action
    //
    void attackPrint() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the weapon of the enemy
    ///
    /// @return the weapon of the enemy
    //
    std::shared_ptr<Weapon> getWeapon() const override {return weapon_;};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Default Destructor
    //
    ~Enemy() = default;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns if the base class character is an enemy. Always true (Replaces typeof)
    ///
    /// @return true
    //
    bool isEnemy() const override{ return true; }
};

#endif //ENEMY_HPP
