//----------------------------------------------------------------------------------------------------------------------
//
// The TreasureChest class represents a treasure chest in the game world. It is a subclass of the Entity class and
// contains a boolean value that indicates whether the treasure chest is locked and an integer value that indicates the
// minimum value of the treasure chest. The inventory stores the loot of the treasure chest.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef TREASURECHEST_HPP
#define TREASURECHEST_HPP

#define TREASURE_CHEST_ABBREVIATION 'T'

#include "Entity.hpp"

class TreasureChest : public Entity
{
  private:
    inline static int treasure_chest_count_ = 0;
    bool is_locked_;
    int min_value_;

  public:
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the TreasureChest class
    /// @param min_value the minimum value to open the treasure chest
    //
    TreasureChest(int min_value, map<string, int> loot) : Entity(treasure_chest_count_++, TREASURE_CHEST_ABBREVIATION,
      loot), is_locked_(true), min_value_(min_value) {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the TreasureChest class
    //
    ~TreasureChest() override {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the treasure chest is locked
    /// @return true if the treasure chest is locked, false otherwise
    //
    bool isLocked() const { return is_locked_; }
};

#endif //TREASURECHEST_HPP