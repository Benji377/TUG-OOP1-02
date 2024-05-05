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
#include <memory>

#include "IO.hpp"
#include "Dungeon.hpp"

class Game
{
  private:
  std::unique_ptr<Dungeon> dungeon_;

  public:
  void parseConfigs(char* dungeon_config_file, char* story_config_file);
  Game() : dungeon_(new Dungeon) {};
  Game(Game&) = delete;
  ~Game() = default;

};


#endif