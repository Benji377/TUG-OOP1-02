
//------------------------------------------------------------------------------------------------------------------
//
// SwitchCommand class: This class is a command performs a switch for Q learning
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef SWITCHCOMMAND_HPP
#define SWITCHCOMMAND_HPP

#include "Command.hpp"

class SwitchCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  SwitchCommand(Game* game) : Command(game) {}
  ~SwitchCommand() override {};
  SwitchCommand(const SwitchCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the Map text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif