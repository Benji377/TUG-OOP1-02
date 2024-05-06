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
#include "../items/Potion.hpp"
#include "../items/Weapon.hpp"
#include "../items/Armor.hpp"
#include "../items/Ammunition.hpp"

class Inventory
{
  std::vector<Potion> potions_;
  std::vector<Weapon> weapons_;
  std::vector<Armor> armor_;
  std::vector<Ammunition> ammunition_;

  public:
    Inventory() = default;
    Inventory(const Inventory& other) = delete;
    Inventory& operator=(const Inventory& other) = delete;
    ~Inventory();
};


#endif //INVENTORY_HPP
