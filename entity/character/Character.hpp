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
    Armor* armor_;
    Weapon* weapon_;
    Inventory* inventory_;
    int base_armor_;
    int strength_;
    int vitality_;
  public:
    Character(int id, char abbreviation) : Entity(id, abbreviation, false) {}
    // Getters
    [[nodiscard]] std::string get_type_name() const { return type_name_; }
    [[nodiscard]] int get_maximum_health() const { return maximum_health_; }
    [[nodiscard]] int get_health() const { return health_; }
    [[nodiscard]] Armor* get_armor() const { return armor_; }
    [[nodiscard]] Weapon* get_weapon() const { return weapon_; }
    [[nodiscard]] int get_base_armor() const { return base_armor_; }
    [[nodiscard]] int get_strength() const { return strength_; }
    [[nodiscard]] int get_vitality() const { return vitality_; }
    // Setters
    void set_maximum_health(int maximum_health) { maximum_health_ = maximum_health; }
    void set_health(int health) { health_ = health; }
    void set_armor(Armor* armor) { armor_ = armor; }
    void set_weapon(Weapon* weapon) { weapon_ = weapon; }
    void set_base_armor(int base_armor) { base_armor_ = base_armor; }
    void set_strength(int strength) { strength_ = strength; }
    void set_vitality(int vitality) { vitality_ = vitality; }
    // Methods
    virtual int move(int row, int column) = 0;                // Missn et pure virtual san, isch la temporär
    virtual void attack(Character& target, int damage) = 0;   // Missn et pure virtual san, isch la temporär
    virtual void take_damage(int damage) = 0;                 // Missn et pure virtual san, isch la temporär
    virtual ~Character() = default; // Fixed error? Mogsch la ändon wennis ondos hom willsch
};


#endif //CHARACTER_HPP
