#ifndef USECOMMAND_HPP
#define USECOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// LootCommand class: This class is a command that toggles map
///
//-----------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class UseCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  UseCommand(Game* game) : Command(game) {}
  ~UseCommand() override {};
  UseCommand(const UseCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. Executes loot functionality
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif