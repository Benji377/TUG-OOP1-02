//----------------------------------------------------------------------------------------------------------------------
//
// LOrem ipsum
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef TREASURECHEST_HPP
#define TREASURECHEST_HPP

#include "Entity.hpp"

class TreasureChest : public Entity
{
  private:
    inline static int treasure_chest_count_ = 0;
    bool is_locked_;
    int min_value_;

  public:
    TreasureChest(int min_value) : Entity(treasure_chest_count_++, 'T', true), is_locked_(true), min_value_(min_value){}
};

#endif //TREASURECHEST_HPP