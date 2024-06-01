//------------------------------------------------------------------------------------------------------------------
//
// LeaveCommand class: This class represents a command prints the Leave of specified player
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef LEAVECOMMAND_HPP
#define LEAVECOMMAND_HPP

#include "Command.hpp"

class LeaveCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Positions class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  LeaveCommand(Game* game) : Command(game) {}
  ~LeaveCommand() override {};
  LeaveCommand(const LeaveCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then prints the Leave of the player.
  ///
  //-----------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif