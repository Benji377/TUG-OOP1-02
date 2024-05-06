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
#include "../entity/Entity.hpp"

class Character: public Entity
{
  protected:
    char type_;
    std::string type_name_;
    int maximum_health_;
    int health_;
    Armor armor_;
    Weapon weapon_;
    int strength_;
    int vitality_;
  public:
    virtual int move(int row, int column);
    virtual void attack(Character& target, int damage);
    virtual void take_damage(int damage);
    virtual ~Character();
};


#endif //CHARACTER_HPP
