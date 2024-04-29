//---------------------------------------------------------------------------------------------------------------------
//
// Item is an abstract class. It is the base class for all items in the game. It contains the attributes abbreviation_,
// name_ and dice_. The abbreviation is a short string that represents the item. The name is the full name of the item.
// The dice_ is a Dice object that represents the dice used to determine the effect of the item. Can be null on
// potions or items that have a fixed value like armor.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include "../utility/Dice.cpp"

class Item
{
  protected:
    std::string abbreviation_;
    std::string name_;
    Dice *dice_;
};


#endif //ITEM_HPP
