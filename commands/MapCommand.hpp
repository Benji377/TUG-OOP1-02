#ifndef MAPCOMMAND_HPP
#define MAPCOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// MapCommand class: This class is a command that toggles map
///
//-----------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class MapCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  MapCommand(Game* game) : Command(game) {}
  ~MapCommand() override {};
  MapCommand(const MapCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the Map text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif