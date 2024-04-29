#include <string>
#include "../utility/Dice.cpp"

#ifndef ITEM_HPP
#define ITEM_HPP

///---------------------------------------------------------------------------------------------------------------------
/// Abstract Item class, mother class of all the items like weapons, armor, etc.
///
class Item {
  protected:
    std::string abbreviation_;
    std::string name_;
    Dice dice_;
};


#endif //ITEM_HPP
