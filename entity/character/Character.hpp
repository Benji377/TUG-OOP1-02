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
    Armor* armor_; // Temporarily changed to a pointer
    Weapon* weapon_; // Temporarily changed to a pointer
    int base_armor_;
    int strength_;
    int vitality_;
  public:
    Character(int id, char abbreviation) : Entity(id, abbreviation, false) {} // Temporary constructor
    virtual int move(int row, int column) = 0;                // Missn et pure virtual san, isch la temporär
    virtual void attack(Character& target, int damage) = 0;   // Missn et pure virtual san, isch la temporär
    virtual void take_damage(int damage) = 0;                 // Missn et pure virtual san, isch la temporär
    virtual ~Character() = default; // Fixed error? Mogsch la ändon wennis ondos hom willsch
};


#endif //CHARACTER_HPP
