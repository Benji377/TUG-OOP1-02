//----------------------------------------------------------------------------------------------------------------------
//
// The Door class represents a door in the game world. It is a subclass of the Entity class and contains a boolean value
// that indicates whether the door is locked and an integer value that indicates the room the door leads to.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef DOOR_HPP
#define DOOR_HPP

#define DOOR_ABBREVIATION 'D'

#include "Entity.hpp"

class Door : public Entity
{
  private:
    bool is_locked_;
    int leads_to_;

  public:
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Door class
    /// @param leads_to the room the door leads to
    //
    Door(int leads_to) : Entity(leads_to, DOOR_ABBREVIATION, false), is_locked_(true), leads_to_(leads_to) {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the Door class
    //
    ~Door() override {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the door is locked
    /// @return true if the door is locked, false otherwise
    //
    bool isLocked() const { return is_locked_; }
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the room the door leads to
    /// @return the room the door leads to
    //
    int getLeadsTo() const { return leads_to_; }
};

#endif //DOOR_HPP