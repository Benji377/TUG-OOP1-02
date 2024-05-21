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

using std::string;
using std::map;

class Entity
{
  protected:
    int id_;
    char abbreviation_;
    map<string, int> loot_;
    bool is_lootable_;

  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Entity class without loot
    ///
    /// @param id the id of the entity
    /// @param abbreviation the abbreviation of the entity
    /// @param is_lootable if the entity is lootable, default is false
    //
    Entity(int id, char abbreviation, bool is_lootable = false) : id_(id), abbreviation_(abbreviation),
          is_lootable_(is_lootable) {};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Entity class with loot. Sets is_lootable to true
    ///
    /// @param id the id of the entity
    /// @param abbreviation the abbreviation of the entity
    /// @param loot the loot of the entity
    //
    Entity(int id, char abbreviation, map<string, int> loot) : id_(id), abbreviation_(abbreviation), loot_(loot),
          is_lootable_(true) {};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the Entity class. Default destructor
    //
    virtual ~Entity() = default;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the abbreviation of the entity
    ///
    /// @return the abbreviation of the entity
    //
    virtual char getAbbreviation() const { return abbreviation_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the id of the entity
    ///
    /// @return the id of the entity
    //
    virtual int getId() const { return id_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to check if the entity is lootable
    ///
    /// @return true if the entity is lootable, false otherwise
    //
    virtual bool isLootable() const { return is_lootable_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the loot of the entity. Its a map of item abbreviations and the amount of the item.
    /// So we can recreate it once the entity is looted and add the character specific values.
    ///
    /// @return the loot of the entity
    //
    virtual map<string, int> getLoot() const { return loot_; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to check if the entity is a character. Default is false.
    ///
    /// @return true if the entity is a character, false otherwise
    //
    virtual bool isCharacter() const { return false; }
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to check if the entity is an enemy. Default is false.
    ///
    /// @return true if the entity is an enemy, false otherwise
    //
    virtual bool isEnemy() const { return false; }
};

#endif //ENTITY_HPP
