//----------------------------------------------------------------------------------------------------------------------
//
// The DeathLocation class represents a death location in the game world. It is by default lootable. The inventory
// stores the loot of the death location.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef DEATHLOCATION_HPP
#define DEATHLOCATION_HPP

#define DEATH_LOCATION_ABBREVIATION 'X'

#include "Entity.hpp"

class DeathLocation : public Entity
{
  private:
    inline static int death_location_count_ = 0;

  public:
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the DeathLocation class
    //
    DeathLocation() : Entity(death_location_count_++, DEATH_LOCATION_ABBREVIATION, true) {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the DeathLocation class
    //
    ~DeathLocation() override {}
};

#endif //DEATHLOCATION_HPP