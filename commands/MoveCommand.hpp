#ifndef MOVECOMMAND_HPP
#define MOVECOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// MoveCommand class: This class is a command that toggles map
///
//-----------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class MoveCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  MoveCommand(Game* game) : Command(game) {}
  ~MoveCommand() override {};
  MoveCommand(const MoveCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the Map text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif