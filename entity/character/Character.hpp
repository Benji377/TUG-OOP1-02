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
    Effect resistant_to_; // Potion effect enum
    std::shared_ptr<Armor> armor_;
    std::shared_ptr<Weapon> weapon_;
    std::shared_ptr<Inventory> inventory_;
    int base_armor_;
    int strength_;
    int vitality_;
  public:
    Character(int id, char abbreviation) : Entity(id, abbreviation, false) {}
    // Getters
    [[nodiscard]] std::string getTypeName() const { return type_name_; }
    [[nodiscard]] int getMaximumHealth() const { return maximum_health_; }
    [[nodiscard]] int getHealth() const { return health_; }
    [[nodiscard]] std::shared_ptr<Armor> getArmor() const { return armor_; }
    [[nodiscard]] std::shared_ptr<Weapon> getWeapon() const { return weapon_; }
    
    [[nodiscard]] std::shared_ptr<Inventory> getInventory() const { return inventory_; }
    //Nur fürs testen hinzugefügt, gerne wieder löschen -Hanno

    [[nodiscard]] int getBaseArmor() const { return base_armor_; }
    [[nodiscard]] int getStrength() const { return strength_; }
    [[nodiscard]] int getVitality() const { return vitality_; }
    bool isCharacter() const override { return true; }
    // Setters
    void setMaximumHealth(int maximum_health) { maximum_health_ = maximum_health; }
    void setHealth(int health) { health_ = health; }
    void setArmor(std::shared_ptr<Armor> armor) { armor_ = armor; }
    void setWeapon(std::shared_ptr<Weapon> weapon) { weapon_ = weapon; }
    void setBaseArmor(int base_armor) { base_armor_ = base_armor; }
    void setStrength(int strength) { strength_ = strength; }
    void setVitality(int vitality) { vitality_ = vitality; }
    // Methods
    virtual int move(int row, int column) = 0;                // Missn et pure virtual san, isch la temporär
    virtual void attack(Character& target, int damage) = 0;   // Missn et pure virtual san, isch la temporär
    virtual void takeDamage(int damage) = 0;                 // Missn et pure virtual san, isch la temporär
    virtual ~Character() = default;
};


#endif //CHARACTER_HPP
