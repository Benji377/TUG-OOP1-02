//------------------------------------------------------------------------------------------------------------------
//
// Quit class: This class represents a command that quits the game
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "Command.hpp"

class QuitCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  QuitCommand(Game* game) : Command(game) {}
  ~QuitCommand() override {};
  QuitCommand(const QuitCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then toggles the is_active_ status of the game
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif