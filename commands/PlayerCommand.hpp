#ifndef PLAYERCOMMAND_HPP
#define PLAYERCOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// PlayerCommand class: This class represents a command prints stats of a player
///
//------------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class PlayerCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Player class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  PlayerCommand(Game* game) : Command(game) {}
  ~PlayerCommand() override {};
  PlayerCommand(const PlayerCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then prints the stats of the players.
  ///
  //-----------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif