#ifndef POSITIONSCOMMAND_HPP
#define POSITIONSCOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// PositionsCommand class: This class represents a command prints the positions of enemies and players. 
//  Gets initialised with the address of the game.
///
//------------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class PositionsCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Positions class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  PositionsCommand(Game* game) : Command(game) {}
  ~PositionsCommand() override {};
  PositionsCommand(const PositionsCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then prints the positions of the players.
  ///
  //-----------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif