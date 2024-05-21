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
#include "../utility/Dice.hpp"
#include <memory>


class Item
{
protected:
  std::string abbreviation_;
  std::string name_;
  std::shared_ptr<Dice> dice_;
public:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the Item abbreviation.
  ///
  /// @return The abbreviation of the item.
  //
  std::string getAbbreviation() const { return abbreviation_; }

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the Item name.
  ///
  /// @return The name of the item.
  //
  std::string getName() const { return name_; }

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the Dice object of the item.
  ///
  /// @return The Dice object of the item.
  //
  std::shared_ptr<Dice> getDice() const { return dice_;};

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Sets the Dice object of the item.
  ///
  /// @param dice The Dice object to set.
  //
  void setDice(std::shared_ptr<Dice> dice) {dice_ = dice;};

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns true if the item is an ammunition. Required for sorting
  ///
  /// @return True if the item is an ammunition.
  //
  bool getIsAmmunition() const { return getAbbreviation() == "ARRW" || getAbbreviation() == "BOLT"; }
};

#endif //ITEM_HPP
