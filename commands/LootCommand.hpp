//------------------------------------------------------------------------------------------------------------------
//
// LootCommand class: This class is a command that performs loot action
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef LOOTCOMMAND_HPP
#define LOOTCOMMAND_HPP

#include "Command.hpp"

class LootCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  LootCommand(Game* game) : Command(game) {}
  ~LootCommand() override {};
  LootCommand(const LootCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. Executes loot functionality
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif