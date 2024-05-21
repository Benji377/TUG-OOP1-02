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
    DamageType resistant_to_;
    std::shared_ptr<Armor> armor_;
    std::shared_ptr<Weapon> weapon_;
    std::shared_ptr<Inventory> inventory_;
    int base_armor_;
    int strength_;
    int vitality_;
    bool is_dead_ = false;
  public:
    Character(int id, char abbreviation) : Entity(id, abbreviation, false) {}
    // Getters
    std::string getTypeName() const { return type_name_; }
    int getMaximumHealth() const { return maximum_health_; }
    int getHealth() const { return health_; }
    std::shared_ptr<Armor> getArmor() const { return armor_; }
    bool isDead() const { return is_dead_; }

    std::shared_ptr<Inventory> getInventory() const { return inventory_; }
    //Nur fürs testen hinzugefügt, gerne wieder löschen -Hanno

    int getBaseArmor() const { return base_armor_; }
    int getStrength() const { return strength_; }
    int getVitality() const { return vitality_; }
    DamageType getResistantTo() const { return resistant_to_; }
    bool isCharacter() const override { return true; }
    // Setters
    void setMaximumHealth(int maximum_health) { maximum_health_ = maximum_health; }
    void setHealth(int health) { health_ = health; }
    void setArmor(std::shared_ptr<Armor> armor) { armor_ = armor; }
    void setWeapon(std::shared_ptr<Weapon> weapon) { weapon_ = weapon; }
    void setBaseArmor(int base_armor) { base_armor_ = base_armor; }
    void setStrength(int strength) { strength_ = strength; }
    void setVitality(int vitality) { vitality_ = vitality; }
    void kill() { is_dead_ = true; }
    // Methods
    virtual int getAttackDamage() = 0;
    virtual int takeDamage(int damage, DamageType damageType) = 0;
    virtual std::shared_ptr<Weapon> getWeapon() const = 0;
    virtual void simplePrint() const = 0;
    virtual ~Character() = default;

};


#endif //CHARACTER_HPP
