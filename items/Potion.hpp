//---------------------------------------------------------------------------------------------------------------------
//
// The class Potion is a derived class of the class Item. It represents a potion in the game. A potion can either be
// a health potion or a resistance potion. Health potions restore health and are consumed on use. Resistance potions
// increase the resistance of the player and are active until the room is changed.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#include "Item.hpp"

#ifndef POTION_HPP
#define POTION_HPP

enum class Effect {HEALTH, FIRE, COLD, FORCE, ACID};


class Potion: public Item
{
 Effect effect_;
};


#endif //POTION_HPP
