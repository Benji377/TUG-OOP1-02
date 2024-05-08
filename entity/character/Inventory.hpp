//---------------------------------------------------------------------------------------------------------------------
//
// The Inventory class can represent the items of a character or loot of enemies and objects.
// It contains vectors for potions, weapons, armor and ammunition, the reason being to keep the items separated.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include "../../items/Potion.hpp"
#include "../../items/Weapon.hpp"
#include "../../items/Armor.hpp"
#include "../../items/Ammunition.hpp"

class Inventory
{
  std::vector<std::shared_ptr<Potion>> potions_;
  std::vector<std::shared_ptr<Weapon>> weapons_;
  std::vector<std::shared_ptr<Armor>> armor_;
  std::vector<std::shared_ptr<Ammunition>> ammunition_;

  public:
    Inventory() = default;
    explicit Inventory(std::map<std::string, int>& inventory);
    int parseInventory(std::map<std::string, int>& inventory);
    void addPotion(std::shared_ptr<Potion> potion);
    void addWeapon(std::shared_ptr<Weapon> weapon);
    void addArmor(std::shared_ptr<Armor> armor);
    void addAmmunition(std::shared_ptr<Ammunition> ammunition);
    int removeItem(std::shared_ptr<Item> item);
    int useAmmunition(std::string& abbreviation);
    std::shared_ptr<Potion> getPotion(std::string& abbreviation);
    std::shared_ptr<Weapon> getWeapon(std::string& abbreviation);
    std::shared_ptr<Armor> getArmor(std::string& abbreviation);
    std::shared_ptr<Ammunition> getAmmunition(std::string& abbreviation);
    Inventory(const Inventory& other) = default;
    Inventory& operator=(const Inventory& other) = delete;
    ~Inventory();
};


#endif //INVENTORY_HPP
