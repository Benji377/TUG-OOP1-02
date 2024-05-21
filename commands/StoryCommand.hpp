#ifndef STORYCOMMAND_HPP
#define STORYCOMMAND_HPP

//------------------------------------------------------------------------------------------------------------------
///
/// StoryCommand class: Toggles story
///
//-----------------------------------------------------------------------------------------------------------------
#include "Command.hpp"

class StoryCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  StoryCommand(Game* game) : Command(game) {}
  ~StoryCommand() override {};
  StoryCommand(const StoryCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the Map text
  ///
  //------------------------------------------------------------------------------------------------------------------
  void execute(std::vector<std::string> params) override;
};


#endif