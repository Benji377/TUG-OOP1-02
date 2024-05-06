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

#include "../dungeon/Dungeon.hpp"
#include "../commands/CommandParser.hpp"
#include "../utility/Props.hpp"
#include "../utility/IO.hpp"
#include "StorySegment.hpp"

using std::vector;

enum class Phase
{
  ACTION,
  ENEMY,
  END
};

class Game
{
  private:
    vector<StorySegment> story;
    std::unique_ptr<CommandParser> parser_;
    Dungeon dungeon_;
    Phase current_phase_;
    bool is_running_;
    bool map_output_active_;
    bool story_output_active_;

  public:
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;
    ~Game(){};
    Game(char *dungeon_path, char *config_path);

    void toggleGame();
    bool isRunning() const;
    void toggleStoryOutput();
    void start();
    void doCommand();
    void doCommand(string input);
};


#endif //GAME_HPP