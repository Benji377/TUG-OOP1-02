//------------------------------------------------------------------------------------------------------------------
//
// PlayerCommand class: This class represents a command prints stats of a player
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef PLAYERCOMMAND_HPP
#define PLAYERCOMMAND_HPP

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
  /// Then prints the stats of the player.
  ///
  //-----------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif