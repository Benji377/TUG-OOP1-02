//----------------------------------------------------------------------------------------------------------------------
//
// The DeathLocation class represents a death location in the game world. It is by default lootable.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//

#ifndef DEATHLOCATION_HPP
#define DEATHLOCATION_HPP

#include "Entity.hpp"

const char DEATH_LOCATION_ABBREVIATION = 'X';

class DeathLocation : public Entity
{
  private:
    inline static int death_location_count_ = 0;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the DeathLocation class
    /// @param loot the loot of the death location
    //
    DeathLocation(map<string, int> loot) : Entity(death_location_count_++, DEATH_LOCATION_ABBREVIATION, loot) {}
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the DeathLocation class
    //
    ~DeathLocation() override { death_location_count_--; }
};

#endif //DEATHLOCATION_HPP