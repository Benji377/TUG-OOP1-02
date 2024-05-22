//------------------------------------------------------------------------------------------------------------------
//
// HelpCommand class: This class represents a command that provides help information.
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef HELPCOMMAND_HPP
#define HELPCOMMAND_HPP

#include "Command.hpp"

class HelpCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Help class calls Constructor of command class. As game ptr is not needed for help, it initialises
  /// the game for this to the nullptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  HelpCommand() : Command(nullptr) {};
  ~HelpCommand() override {};
  HelpCommand(const HelpCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the help text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif