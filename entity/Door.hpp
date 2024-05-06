//----------------------------------------------------------------------------------------------------------------------
//
// LOrem ipsum
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef DOOR_HPP
#define DOOR_HPP

#include "Entity.hpp"

class Door : public Entity
{
  private:
    bool is_locked_;
    int leads_to_;

  public:
    Door(int leads_to) : Entity(0, 'D', false), is_locked_(true), leads_to_(leads_to) {}
};

#endif //DOOR_HPP