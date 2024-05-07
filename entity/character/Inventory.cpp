#include "Inventory.hpp"

void Inventory::addPotion(Potion *potion)
{
  potions_.push_back(potion);
}

void Inventory::addWeapon(Weapon *weapon)
{
  weapons_.push_back(weapon);
}

void Inventory::addArmor(Armor *armor)
{
  armor_.push_back(armor);
}

void Inventory::addAmmunition(Ammunition *ammunition)
{
  ammunition_.push_back(ammunition);
}

int Inventory::removeItem(Item *item)
{
  // Loop through all the items and remove the item if the abbreviation matches
  for (auto it = potions_.begin(); it != potions_.end(); ++it)
  {
    if ((*it)->getAbbreviation() == item->getAbbreviation())
    {
      potions_.erase(it);
      return 0;
    }
  }
  for (auto it = weapons_.begin(); it != weapons_.end(); ++it)
  {
    if ((*it)->getAbbreviation() == item->getAbbreviation())
    {
      weapons_.erase(it);
      return 0;
    }
  }
  for (auto it = armor_.begin(); it != armor_.end(); ++it)
  {
    if ((*it)->getAbbreviation() == item->getAbbreviation())
    {
      armor_.erase(it);
      return 0;
    }
  }
  for (auto it = ammunition_.begin(); it != ammunition_.end(); ++it)
  {
    if ((*it)->getAbbreviation() == item->getAbbreviation())
    {
      ammunition_.erase(it);
      return 0;
    }
  }
  return 1;
}

// Uses up a single ammunition type and returns how many there are left. 0 if the item is deleted or -1 if an error occurred.
int Inventory::useAmmunition(std::string &abbreviation)
{
  for (auto & ammunition : ammunition_)
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

Potion *Inventory::getPotion(std::string &abbreviation)
{
  for (auto & potion : potions_)
  {
    if (potion->getAbbreviation() == abbreviation)
    {
      return potion;
    }
  }
  return nullptr;
}

Weapon *Inventory::getWeapon(std::string &abbreviation)
{
  for (auto & weapon : weapons_)
  {
    if (weapon->getAbbreviation() == abbreviation)
    {
      return weapon;
    }
  }
  return nullptr;
}

Armor *Inventory::getArmor(std::string &abbreviation)
{
  for (auto & armor : armor_)
  {
    if (armor->getAbbreviation() == abbreviation)
    {
      return armor;
    }
  }
  return nullptr;
}

Ammunition *Inventory::getAmmunition(std::string &abbreviation)
{
  for (auto & ammunition : ammunition_)
  {
    if (ammunition->getAbbreviation() == abbreviation)
    {
      return ammunition;
    }
  }
  return nullptr;
}

Inventory::~Inventory()
{
  for (auto & potion : potions_)
  {
    delete potion;
  }
  for (auto & weapon : weapons_)
  {
    delete weapon;
  }
  for (auto & armor : armor_)
  {
    delete armor;
  }
  for (auto & ammunition : ammunition_)
  {
    delete ammunition;
  }
}
