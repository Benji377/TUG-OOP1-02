#ifndef INVENTORYCOMMAND_HPP
#define INVENTORYCOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// InventoryCommand class: This class represents a command prints the inventory of specified player
///
//------------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class InventoryCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Positions class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  InventoryCommand(Game* game) : Command(game) {}
  ~InventoryCommand() override {};
  InventoryCommand(const InventoryCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then prints the positions of the players.
  ///
  //-----------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif