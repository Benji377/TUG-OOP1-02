#include "Props.hpp"

std::shared_ptr<Potion> Props::craftPotion(std::string abbreviation)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(CSVParser::POTIONS_CSV_PATH, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("[PROPS] Invalid abbreviation: \" " + abbreviation + " \" for potion");
  }
  if (row[2].empty())
  {
    // It's a health potion
    return std::make_shared<Potion>(abbreviation, row[1], "health", std::make_shared<Dice>(row[3]));
  }
  else
  {
    // It's a resistance potion
    return std::make_shared<Potion>(abbreviation, row[1], row[2], nullptr);
  }
}

std::shared_ptr<Ammunition> Props::craftAmmunition(std::string abbreviation, int amount)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(CSVParser::AMMUNITION_CSV_PATH, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("[PROPS] Invalid abbreviation: \" " + abbreviation + " \" for ammunition");
  }
  // Convert the string to a vector of strings splitting by the comma
  std::vector<std::string> weapons = Utils::splitString(row[2], ",");
  return std::make_shared<Ammunition>(abbreviation, row[1], weapons, amount);
}

std::shared_ptr<Armor> Props::craftArmor(std::string abbreviation, int vitality)
{
  std::vector<std::string> row = CSVParser::getRowByAbbreviation(CSVParser::ARMOR_CSV_PATH, abbreviation);
  if (row.empty())
  {
    throw std::invalid_argument("[PROPS] Invalid abbreviation: \" " + abbreviation + " \" for armor");
  }
  if (row.size() > 3 && !row[3].empty())
  {
    // Very specific, comes from config and is a special case
    if (row[3] == "min(VIT, 2)")
    {
      // Calculate the minimum of the vitality and 2
      int armor_value = std::stoi(row[2]) + std::min(vitality, 2);
      return std::make_shared<Armor>(abbreviation, row[1], armor_value);
    }
    else
    {
      return std::make_shared<Armor>(abbreviation, row[1], std::stoi(row[2]) + vitality);
    }
  }
  return std::make_shared<Armor>(abbreviation, row[1], std::stoi(row[2]));
}

std::shared_ptr<Weapon> Props::craftWeapon(std::string abbreviation, char character, int strength, int vitality)
{
  std::vector<std::string> row;
  if (character == 'L' || character == 'W')
  {
    row = CSVParser::getRowByAbbreviation(CSVParser::WEAPON_CSV_PATH, abbreviation + "-X");
    if (row.empty())
    {
      row = CSVParser::getRowByAbbreviation(CSVParser::WEAPON_CSV_PATH, abbreviation);
    }
    else
    {
      return std::make_shared<Weapon>(abbreviation, row[1], std::make_shared<Dice>(row[5]),
                                      std::make_shared<DamagePattern>(row[4]), row[2], row[3], 0);
    }
  }
  else
  {
    row = CSVParser::getRowByAbbreviation(CSVParser::WEAPON_CSV_PATH, abbreviation);
  }
  if (row.empty())
  {
    throw std::invalid_argument("[PROPS] Invalid abbreviation: \" " + abbreviation + " \" for weapon");
  }
  if (row[6].empty())
  {
    // It's a weapon that doesn't require a character
    return std::make_shared<Weapon>(abbreviation, row[1], std::make_shared<Dice>(row[5]),
                                    std::make_shared<DamagePattern>(row[4]), row[2], row[3], 0);
  }
  else
  {
    if (row[6] == "STR")
    {
      // Calculate the damage based on the strength
      return std::make_shared<Weapon>(abbreviation, row[1], std::make_shared<Dice>(row[5]),
                                      std::make_shared<DamagePattern>(row[4]), row[2], row[3], strength);
    }
    else if (row[6] == "VIT")
    {
      // Calculate the damage based on the vitality
      return std::make_shared<Weapon>(abbreviation, row[1], std::make_shared<Dice>(row[5]),
                                      std::make_shared<DamagePattern>(row[4]), row[2], row[3], vitality);
    }
    else
    {
      throw std::invalid_argument("[PROPS] Invalid character attribute: \" " + row[6] + " \" for weapon");
    }
  }
}