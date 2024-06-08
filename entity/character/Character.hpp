//---------------------------------------------------------------------------------------------------------------------
//
// The Character abstract class is the base class for both the Player and Enemy classes. It contains common
// attributes and methods that are shared between the two classes.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "../Entity.hpp"

class Character: public Entity
{
  protected:
    std::string type_name_;
    int maximum_health_;
    int health_;
    // Determined by the Resistance Potion
    DamageType resistant_to_;
    // Equipped Armor and Weapon
    std::shared_ptr<Armor> armor_;
    std::shared_ptr<Weapon> weapon_;
    // Inventory / Loot for Enemies
    std::shared_ptr<Inventory> inventory_;
    // Base Stats
    int base_armor_;
    int strength_;
    int vitality_;
    bool is_dead_ = false;
  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor initializes the character with an id and an abbreviation. The abbreviation is used to
    /// retrieve the character stats from the character database.
    ///
    /// @param id The id of the character.
    /// @param abbreviation The abbreviation of the character.
    //
    Character(int id, char abbreviation) : Entity(id, abbreviation, false) {}
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the type of character, the longer name of the abbreviation. Not the player name.
    ///
    /// @return The type of character.
    //
    std::string getTypeName() const { return type_name_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the maximum health of the character.
    ///
    /// @return The maximum health of the character.
    //
    int getMaximumHealth() const { return maximum_health_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the current health of the character.
    ///
    /// @return The current health of the character.
    //
    int getHealth() const { return health_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the currently equipped armor of the character.
    ///
    /// @return The currently equipped armor of the character.
    //
    std::shared_ptr<Armor> getArmor() const { return armor_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns true if the character is dead, false otherwise. Gets set by the Game.
    ///
    /// @return True if the character is dead, false otherwise.
    //
    bool isDead() const { return is_dead_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the inventory of the character.
    ///
    /// @return The inventory of the character.
    //
    std::shared_ptr<Inventory> getInventory() const { return inventory_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the base armor of the character.
    ///
    /// @return The base armor of the character.
    //
    int getBaseArmor() const { return base_armor_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the strength of the character.
    ///
    /// @return The strength of the character.
    //
    int getStrength() const { return strength_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the vitality of the character.
    ///
    /// @return The vitality of the character.
    //
    int getVitality() const { return vitality_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the damage type the character is resistant to.
    ///
    /// @return The damage type the character is resistant to.
    //
    DamageType getResistantTo() const { return resistant_to_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Since the character class is based on the Entity class, this method returns true to help us determine if the
    /// entity is a character or if its something else.
    ///
    /// @return True if the entity is a character, always
    //
    bool isCharacter() const override { return true; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the maximum health of the character.
    ///
    /// @param maximum_health The maximum health of the character.
    //
    void setMaximumHealth(int maximum_health) { maximum_health_ = maximum_health; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the current health of the character.
    ///
    /// @param health The current health of the character.
    //
    void setHealth(int health) { health_ = health; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the currently active armor of the character.
    ///
    /// @param armor The currently active armor of the character.
    //
    void setArmor(std::shared_ptr<Armor> armor) { armor_ = armor; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the currently active weapon of the character.
    ///
    /// @param weapon The currently active weapon of the character.
    //
    void setWeapon(std::shared_ptr<Weapon> weapon) { weapon_ = weapon; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the Base armor value of the character.
    ///
    /// @param base_armor The base armor value of the character.
    //
    void setBaseArmor(int base_armor) { base_armor_ = base_armor; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the strength of the character.
    ///
    /// @param strength The strength of the character.
    //
    void setStrength(int strength) { strength_ = strength; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the vitality of the character.
    ///
    /// @param vitality The vitality of the character.
    //
    void setVitality(int vitality) { vitality_ = vitality; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// "Kills" the character by setting the is_dead_ attribute to true.
    //
    void kill() { is_dead_ = true; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the attack damage of the character. This is a pure virtual function and has to be implemented by the
    /// derived classes.
    ///
    /// @return The attack damage of the character.
    //
    virtual int getAttackDamage() = 0;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Makes the Character take damage. This is a pure virtual function and has to be implemented by the derived
    /// classes.
    ///
    /// @param damage The amount of damage the character takes.
    /// @param damageType The type of damage the character takes.
    ///
    /// @return The remaining health of the character.
    //
    virtual int takeDamage(int damage, DamageType damageType) = 0;

    virtual int simulateDamage(int damage, DamageType damageType) = 0;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the currently equipped weapon of the character. This is a pure virtual function and has to be
    /// implemented by the derived classes.
    ///
    /// @return The currently equipped weapon of the character.
    //
    virtual std::shared_ptr<Weapon> getWeapon() const = 0;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the character. This is a pure virtual function and has to be implemented by the derived classes.
    //
    virtual void simplePrint() const = 0;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the Character class.
    //
    virtual ~Character() = default;
};

#endif //CHARACTER_HPP
