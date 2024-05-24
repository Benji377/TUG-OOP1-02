//------------------------------------------------------------------------------------------------------------------
//
// PlayCommand class: This class is a command performs move action
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef PLAYCOMMAND_HPP
#define PLAYCOMMAND_HPP

#include "Command.hpp"

class PlayCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  PlayCommand(Game* game) : Command(game) {}
  ~PlayCommand() override {};
  PlayCommand(const PlayCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the Map text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif