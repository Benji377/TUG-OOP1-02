//---------------------------------------------------------------------------------------------------------------------
//
// The Armor class is a class that represents the armor that a character can wear. It inherits from the Item class.
// Furthermore, it adds the armor value to the item.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "Item.hpp"
#include "../utility/Utils.hpp"


class Armor : public Item
{
  int armor_value_;
public:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This constructor creates an Armor object with the given abbreviation, name and armor value.
  ///
  /// @param abbreviation The abbreviation of the armor.
  /// @param name The name of the armor.
  /// @param armor_value The armor value of the armor.
  //
  Armor(std::string &abbreviation, std::string &name, int armor_value);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This method returns the armor value of the armor.
  //
  int getArmorValue() const;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This method sets the armor value of the armor.
  ///
  /// @param armor_value The armor value of the armor.
  //
  void setArmorValue(int armor_value);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This method is deleted to prevent copying of Armor objects.
  //
  Armor(const Armor &armor) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This method is deleted to prevent copying of Armor objects.
  //
  Armor &operator=(const Armor &armor) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This method is the destructor of the Armor class.
  //
  ~Armor() = default;
};

#endif //ARMOR_HPP
