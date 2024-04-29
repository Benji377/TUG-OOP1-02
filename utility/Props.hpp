#include <string>
#include "../items/Potion.cpp"
#include "../items/Weapon.cpp"
#include "../items/Armor.cpp"
#include "../items/Ammunition.cpp"

#ifndef PROPS_HPP
#define PROPS_HPP

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
  std::string ammunition_csv_path_;
  std::string armor_csv_path_;
  std::string potions_csv_path_;
  std::string weapon_csv_path_;

public:
  static void craftPotion(Potion& potion, std::string& abbreviation);
  static void craftWeapon(Weapon& weapon, std::string& abbreviation);
  static void craftWeapon(Weapon& weapon, std::string& abbreviation, char& character);
  static void craftArmor(Armor& armor, std::string& abbreviation);
  static void craftAmmunition(Ammunition& ammunition, std::string& abbreviation);
};


#endif //PROPS_HPP
