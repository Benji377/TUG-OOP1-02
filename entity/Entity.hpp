//---------------------------------------------------------------------------------------------------------------------
//
// The Entity is an abstract class that represents an object in the game world. Something like a treasure,
// a character, a door, a character or a death location. It normally has an inventory, which can also be used as loot, but
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
#include <map>
#include "character/Inventory.hpp"

class Field;

using std::string;
using std::map;

class Entity {
  protected:
    int id_;
    char abbreviation_;
    map<string, int> loot_;
    bool is_lootable_;

  public:
    Entity(int id, char abbreviation, bool is_lootable = false) : id_(id), abbreviation_(abbreviation),
          is_lootable_(is_lootable) {}
    Entity(int id, char abbreviation, map<string, int> loot) : id_(id), abbreviation_(abbreviation), loot_(loot),
          is_lootable_(true){}
    virtual ~Entity() = default;

    [[nodiscard]] virtual char getAbbreviation() const { return abbreviation_; }
    [[nodiscard]] virtual int getId() const { return id_; }
    [[nodiscard]] virtual bool isLootable() const { return is_lootable_; }
    [[nodiscard]] virtual map<string, int> getLoot() const { return loot_; }
    virtual bool isCharacter() const { return false; }
    virtual bool isEnemy() const { return false; }

};

#endif //ENTITY_HPP
