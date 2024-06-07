//---------------------------------------------------------------------------------------------------------------------
//
// The Player class is derived from the Character class, and it represents the player in the game. It extends the
// base class by adding a name and a resistance attribute. The resistance attribute is used to calculate the damage
// the player takes from an enemy attack.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include <cstring>
#include "../../items/DamagePattern.hpp"

class Player: public Character
{
  std::string name_;
  void initializeInventory();
  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Player class
    ///
    /// @param id the id of the player
    /// @param abbreviation the abbreviation of the player
    /// @param name the name of the player
    //
    Player(int id, char abbreviation, std::string name);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the attack damage of the player
    ///
    /// @return the attack damage of the player
    //
    int getAttackDamage() override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Makes the Player take damage from an attack and reduces the health points accordingly.
    ///
    /// @param damage the damage the player takes
    /// @param damage_type the type of the damage
    ///
    /// @return the remaining health points of the player
    //
    int takeDamage(int damage, DamageType damage_type) override;

    int simulateDamage(int damage, DamageType damage_type) override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to set the resistance of the player. The resistance reduces the damage to 50%
    ///
    /// @param damage_type the type of the damage
    //
    void setResistance(DamageType damage_type);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to use a potion from the inventory of the player
    ///
    /// @param abbreviation the abbreviation of the potion
    ///
    /// @return 0 if the potion was used successfully, 1 if the potion was not found in the inventory
    //
    int usePotion(std::string abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to print the player at a specific position
    ///
    /// @param position the position of the player
    /// @param single_line if true, the printed information is reduced to one line
    //
    void printPlayer(const std::pair<int, int>& position, bool single_line = false) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the name of the player
    ///
    /// @return the name of the player
    //
    std::string getName() const { return name_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to print the player with simple information
    //
    void simplePrint() const override;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to set the active weapon of the player
    ///
    /// @param weapon_abbreviation the abbreviation of the weapon
    //
    void setActiveWeapon(std::string weapon_abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to set the armor of the player
    ///
    /// @param armor_abbreviation the abbreviation of the armor
    //
    void setArmor(std::string armor_abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to print the player without the id
    //
    void simplePrintNoId() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the currently active weapon of the player
    ///
    /// @return the active weapon of the player
    //
    std::shared_ptr<Weapon> getWeapon() const override { return weapon_; };

    std::shared_ptr<Weapon> getBestMeleeWeapon() const;

    std::shared_ptr<Weapon> getBestRangeWeaponWithAmmunition() const;

    std::shared_ptr<Armor> getBestArmor() const;

    bool hasRangeWeaponEquipped() const;

    bool hasMeleeWeapon() const;

    bool hasMeleeWeaponEquipped() const;

    bool hasRangeWeaponWithAmmunition() const;

    bool hasBetterArmor() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the currently active armor of the player
    ///
    /// @return the active armor of the player
    //
    ~Player() = default;
};

#endif //PLAYER_HPP