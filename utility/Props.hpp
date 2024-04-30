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
#include "../items/Potion.cpp"
#include "../items/Weapon.cpp"
#include "../items/Armor.cpp"
#include "../items/Ammunition.cpp"

/*
 * Quick Notes:
 * - The class of each prop is saved as a string like "xdy" where x is the amount of dice and y the dice size.
 * - The weapons can have different properties depending on the character. For character specific properties,
 * use the Abbreviation with an `-X` at the end. The characters that apply are also stored.
 * So basically we store those weapons twice, one for the all characters (default) and extension (-X) for the specific character.
 */
// TODO: Use function overloading to retrieve weapons with or without character
class Props
{
  std::string ammunition_csv_path_ = "../items/csv/ammunition.csv";
  std::string armor_csv_path_ = "../items/csv/armor.csv";
  std::string potions_csv_path_ = "../items/csv/potions.csv";
  std::string weapon_csv_path_ = "../items/csv/weapons.csv";

public:
  static void craftPotion(Potion& potion, std::string& abbreviation);
  // TODO: Weapon either requires the character or the strength and vitality of the player to calculate the damage
  static void craftWeapon(Weapon& weapon, std::string& abbreviation, int strength = 0, int vitality = 0);
  static void craftWeapon(Weapon& weapon, std::string& abbreviation, char& character);
  // TODO: Armor also needs the players vitality to calculate the armor value
  static void craftArmor(Armor& armor, std::string& abbreviation, int vitality = 0);
  static void craftAmmunition(Ammunition& ammunition, std::string& abbreviation);
};


#endif //PROPS_HPP
