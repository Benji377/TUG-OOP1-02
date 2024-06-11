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

#include "Entity.hpp"

const char TREASURE_CHEST_ABBREVIATION = 'T';

class TreasureChest : public Entity
{
  private:
    inline static int treasure_chest_count_ = 0;
    bool is_locked_;
    int min_value_;
    bool unlock_value_too_high_;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the TreasureChest class
    /// @param min_value the minimum value to open the treasure chest
    //
    TreasureChest(int min_value, map<string, int> loot) : Entity(treasure_chest_count_++, TREASURE_CHEST_ABBREVIATION,
      loot), is_locked_(true), min_value_(min_value), unlock_value_too_high_(false) {}
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the TreasureChest class
    //
    ~TreasureChest() override { treasure_chest_count_--; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the treasure chest is locked
    ///
    /// @return true if the treasure chest is locked, false otherwise
    //
    bool isLocked() const { return is_locked_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the minimum value of the treasure chest
    ///
    /// @return the minimum value of the treasure chest
    //
    int getMinValue() const { return min_value_; };

    void setUnlockValueTooHigh(bool set) { unlock_value_too_high_ = set;};
    bool isUnlockValueTooHigh() const { return unlock_value_too_high_;};
};

#endif //TREASURECHEST_HPP