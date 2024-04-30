#include "Props.hpp"
#include "CSVParser.cpp"

std::string ammunition_csv_path_ = "../items/csv/ammunition.csv";
std::string armor_csv_path_ = "../items/csv/armor.csv";
std::string potions_csv_path_ = "../items/csv/potions.csv";
std::string weapon_csv_path_ = "../items/csv/weapons.csv";

Potion *Props::craftPotion(std::string& abbreviation)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(potions_csv_path_, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("Invalid abbreviation");
  }
  if (row[3].empty())
  {
    // It's a health potion
    return new Potion(abbreviation, row[1], (std::string &) "health", new Dice(row[4]));
  }
  else
  {
    // It's a resistance potion
    return new Potion(abbreviation, row[1], row[3], nullptr);
  }
}

Ammunition* Props::craftAmmunition(std::string& abbreviation)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(ammunition_csv_path_, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("Invalid abbreviation");
  }
  // TODO: Somehow implement the weapons here using row[2] as the weapon abbreviation
  // Convert the string to a vector of strings splitting by the comma
  std::vector<std::string> weapons = Utils::splitString(row[2], ",");
  return new Ammunition(abbreviation, row[1], weapons);
}

Armor* Props::craftArmor(std::string& abbreviation, int vitality)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(armor_csv_path_, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("Invalid abbreviation");
  }
  if (row.size() > 3 && !row[3].empty())
  {
    // TODO: This is very specific, leave as is?
    if (row[3] == "min(VIT, 2)")
    {
      // Calculate the minimum of the vitality and 2
      int armor_value = std::stoi(row[2]) + std::min(vitality, 2);
      return new Armor(abbreviation, row[1], armor_value);
    }
    else
    {
      return new Armor(abbreviation, row[1], std::stoi(row[2]) + vitality);
    }
  }
  return new Armor(abbreviation, row[1], std::stoi(row[2]));
}

Weapon* Props::craftWeapon(std::string& abbreviation, char &character)
{
  if (character != 'L' && character != 'W')
  {
    throw std::invalid_argument("Invalid character");
  }
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(weapon_csv_path_, abbreviation + "-X");
  if (row.empty())
  {
    throw std::invalid_argument("Invalid abbreviation");
  }
  return new Weapon(abbreviation, row[1], new Dice(row[5]), new DamagePattern(row[4]), row[2], row[3], 0);
}

Weapon* Props::craftWeapon(std::string& abbreviation, int strength, int vitality)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(weapon_csv_path_, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("Invalid abbreviation");
  }
  if (row[6].empty())
  {
    // It's a weapon that doesn't require a character
    return new Weapon(abbreviation, row[1], new Dice(row[5]), new DamagePattern(row[4]), row[2], row[3], 0);
  }
  else
  {
    if (row[6] == "STR")
    {
      // Calculate the damage based on the strength
      return new Weapon(abbreviation, row[1], new Dice(row[5]), new DamagePattern(row[4]), row[2], row[3], strength);
    }
    else if (row[6] == "VIT")
    {
      // Calculate the damage based on the vitality
      return new Weapon(abbreviation, row[1], new Dice(row[5]), new DamagePattern(row[4]), row[2], row[3], vitality);
    }
  }
  return new Weapon(abbreviation, row[1], new Dice(row[5]), new DamagePattern(row[4]), row[2], row[3], 0);
}