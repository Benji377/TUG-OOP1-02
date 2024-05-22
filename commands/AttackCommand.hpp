//------------------------------------------------------------------------------------------------------------------
//
// AttackCommand class: This class is a command that performs attack ingame
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef ATTACKCOMMAND_HPP
#define ATTACKCOMMAND_HPP

#include "Command.hpp"

class AttackCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //
  AttackCommand(Game* game) : Command(game) {}
  ~AttackCommand() override {};
  AttackCommand(const AttackCommand&) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. Executes attack functionality
  ///
  /// @param params Vector of strings which is the command
  //
  void execute(std::vector<std::string> params) override;
};


#endif