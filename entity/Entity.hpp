//---------------------------------------------------------------------------------------------------------------------
//
// The Entity is an abstract class that represents an object in the game world. Something like a treasure,
// a character, a door or a death location. It normally has an inventory, which can also be used as loot, but
// it doesn't have to. It can also be a simple object that can't be looted or doesn't have an inventory.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <optional>
#include <string>
#include "../character/Inventory.hpp"

class Entity {
  int id_;
  std::optional<Inventory> inventory_;
  bool is_lootable_;
};

#endif //ENTITY_HPP
