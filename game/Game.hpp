//---------------------------------------------------------------------------------------------------------------------
//
// Game class handles most basic game logic.
//
// Group: 068
//
// Author:
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <stdexcept>

#include "../dungeon/Dungeon.hpp"
#include "../commands/CommandParser.hpp"
#include "../utility/Props.hpp"
#include "../utility/IO.hpp"

class Game
{
  private:
  std::unique_ptr<CommandParser> parser_;
  bool is_running_;
  bool story_output_active_;

  public:
  void parseConfigs(char* dungeon_config_file, char* story_config_file);
  Game();
  Game(Game&) = delete;
  ~Game(){};

  void toggleGame();
  void toggleStoryOutput();

  bool isRunning() const;

  void start();
  void doCommand();

};


#endif