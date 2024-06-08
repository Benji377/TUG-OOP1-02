#include "Inventory.hpp"
#include "../../utility/CSVParser.hpp"
#include "../../utility/Props.hpp"

Inventory::Inventory(std::map<std::string, int> &inventory, char player_abbreviation,
                     int player_strength, int player_vitality)
{
  parseInventory(inventory, player_abbreviation, player_strength, player_vitality);
}

void Inventory::addPotion(std::shared_ptr<Potion> potion)
{
  potions_.push_back(potion);
}

void Inventory::addWeapon(std::shared_ptr<Weapon> weapon)
{
  weapons_.push_back(weapon);
}

void Inventory::addArmor(std::shared_ptr<Armor> armor)
{
  armor_.push_back(armor);
}

void Inventory::addAmmunition(std::shared_ptr<Ammunition> ammunition)
{
  // Check if the ammunition with the same abbreviation already exists
  // If it does, simply add the amount of the new ammunition to the existing one and delete the new one
  for (auto &existing_ammunition: ammunition_)
  {
    if (existing_ammunition->getAbbreviation() == ammunition->getAbbreviation())
    {
      existing_ammunition->setAmount(existing_ammunition->getAmount() + ammunition->getAmount());
      return;
    }
  }
  ammunition_.push_back(ammunition);
}

int Inventory::removeItem(std::shared_ptr<Item> item)
{
  // Loop through all the items and remove the item if the abbreviation matches
  for (auto iterator = potions_.begin(); iterator != potions_.end(); ++iterator)
  {
    if ((*iterator)->getAbbreviation() == item->getAbbreviation())
    {
      potions_.erase(iterator);
      return 0;
    }
  }
  for (auto iterator = weapons_.begin(); iterator != weapons_.end(); ++iterator)
  {
    if ((*iterator)->getAbbreviation() == item->getAbbreviation())
    {
      weapons_.erase(iterator);
      return 0;
    }
  }
  for (auto iterator = armor_.begin(); iterator != armor_.end(); ++iterator)
  {
    if ((*iterator)->getAbbreviation() == item->getAbbreviation())
    {
      armor_.erase(iterator);
      return 0;
    }
  }
  for (auto iterator = ammunition_.begin(); iterator != ammunition_.end(); ++iterator)
  {
    if ((*iterator)->getAbbreviation() == item->getAbbreviation())
    {
      ammunition_.erase(iterator);
      return 0;
    }
  }
  return 1;
}

// Uses up a single ammunition type and returns how many there are left. 0 if the item is deleted or -1 if an error occurred.
int Inventory::useAmmunition(std::string &abbreviation)
{
  for (auto &ammunition: ammunition_)
  {
    if (ammunition->getAbbreviation() == abbreviation)
    {
      ammunition->setAmount(ammunition->getAmount() - 1);
      if (ammunition->getAmount() == 0)
      {
        removeItem(ammunition);
        return 0;
      }
      return ammunition->getAmount();
    }
  }
  return -1;
}

std::shared_ptr<Potion> Inventory::getPotion(const std::string &abbreviation)
{
  for (auto &potion: potions_)
  {
    if (potion->getAbbreviation() == abbreviation)
    {
      return potion;
    }
  }
  return nullptr;
}

std::shared_ptr<Weapon> Inventory::getWeapon(const std::string &abbreviation)
{
  for (auto &weapon: weapons_)
  {
    if (weapon->getAbbreviation() == abbreviation)
    {
      return weapon;
    }
  }
  return nullptr;
}

std::shared_ptr<Weapon> Inventory::getRandomWeapon()
{
  if (weapons_.empty())
  {
    return nullptr;
  }
  return weapons_[Oop::Random::getInstance().getRandomNumber(getAllWeapons().size()) - 1];
}

std::shared_ptr<Weapon> Inventory::getRandomWeapon(AttackType attack_type)
{
  std::vector<std::shared_ptr<Weapon>> weapons;
  for (auto &weapon: weapons_)
  {
    if (weapon->getAttackType() == attack_type)
    {
      weapons.push_back(weapon);
    }
  }
  if (weapons.empty())
  {
    return nullptr;
  }
  return weapons[Oop::Random::getInstance().getRandomNumber(weapons.size()) - 1];
}


std::shared_ptr<Armor> Inventory::getArmor(const std::string &abbreviation)
{
  for (auto &armor: armor_)
  {
    if (armor->getAbbreviation() == abbreviation)
    {
      return armor;
    }
  }
  return nullptr;
}

std::shared_ptr<Ammunition> Inventory::getAmmunition(const std::string &abbreviation)
{
  for (auto &ammunition: ammunition_)
  {
    if (ammunition->getAbbreviation() == abbreviation)
    {
      return ammunition;
    }
  }
  return nullptr;
}

int Inventory::parseInventory(std::map<std::string, int> &inventory, char player_abbreviation,
                              int player_strength, int player_vitality)
{
  for (auto &item: inventory)
  {
    std::string parsed_item = CSVParser::getFolderByAbbreviation(item.first);
    if (parsed_item.empty())
    {
      return 1;
    }
    else if (parsed_item == "Ammunition")
    {
      addAmmunition(Props::craftAmmunition(const_cast<std::string &>(item.first), item.second));
    } else
    {
      for (int item_index = 0; item_index < item.second; item_index++)
      {
        if (parsed_item == "Potion")
        {
          addPotion(Props::craftPotion(const_cast<std::string &>(item.first)));
        }
        else if (parsed_item == "Weapon")
        {
          addWeapon(Props::craftWeapon(const_cast<std::string &>(item.first), player_abbreviation,
                                       player_strength, player_vitality));
        }
        else if (parsed_item == "Armor")
        {
          addArmor(Props::craftArmor(const_cast<std::string &>(item.first), player_vitality));
        }
      }
    }
  }
  return 0;
}

std::map<std::string, int> Inventory::getInventoryMapped() const
{
  std::map<std::string, int> inventory;
  for (auto &potion: potions_)
  {
    inventory[potion->getAbbreviation()]++;
  }
  for (auto &weapon: weapons_)
  {
    inventory[weapon->getAbbreviation()]++;
  }
  for (auto &armor: armor_)
  {
    inventory[armor->getAbbreviation()]++;
  }
  for (auto &ammunition: ammunition_)
  {
    inventory[ammunition->getAbbreviation()] += ammunition->getAmount();
  }
  return inventory;
}
