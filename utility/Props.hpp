//---------------------------------------------------------------------------------------------------------------------
//
// The Props class can create Item objects by reading its values from a CSV file. The CSV file is read by the
// CSVReader class and the values are stored in a vector of strings. The values are then parsed and stored in the
// respective Item object. The CSV file is stored in the items/csv folder.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef PROPS_HPP
#define PROPS_HPP

#include <string>
#include "CSVParser.hpp"
#include "../items/Potion.hpp"
#include "../items/Weapon.hpp"
#include "../items/Armor.hpp"
#include "../items/Ammunition.hpp"


class Props
{
public:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This function reads the CSV file and creates a Potion object with the values from the CSV file.
  /// @param abbreviation The abbreviation of the potion
  /// @return The Potion object
  //
  static Potion* craftPotion(std::string& abbreviation);
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This function reads the CSV file and creates a Weapon object with the values from the CSV file. The strength and
  /// vitality values are optional and can be set to 0. If provided, the strength and vitality values are added to the
  /// Weapon damage score.
  /// @param abbreviation The abbreviation of the weapon
  /// @param strength The strength value
  /// @param vitality The vitality value
  /// @return The Weapon object
  //
  static Weapon* craftWeapon(std::string& abbreviation, int strength = 0, int vitality = 0);
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Similar to the other Weapon function, this creates a weapon based on a character. Only 'L' and 'W' are valid.
  /// The character is case sensitive.
  /// @param abbreviation The abbreviation of the weapon
  /// @param character The character
  /// @return The Weapon object
  //
  static Weapon* craftWeapon(std::string& abbreviation, char character);
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This function reads the CSV file and creates an Armor object with the values from the CSV file. The vitality value
  /// is optional and can be set to 0. If provided, the vitality value is added to the Armor value.
  /// @param abbreviation The abbreviation of the armor
  /// @param vitality The vitality value
  /// @return The Armor object
  //
  static Armor* craftArmor(std::string& abbreviation, int vitality = 0);
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// This function reads the CSV file and creates an Ammunition object with the values from the CSV file.
  /// @param abbreviation The abbreviation of the ammunition
  /// @return The Ammunition object
  //
  static Ammunition* craftAmmunition(std::string& abbreviation, int amount = 1);
};


#endif //PROPS_HPP
