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

class Game
{
  private:
    std::vector<StorySegment> story;
    std::unique_ptr<CommandParser> parser_;
    bool is_running_;
    bool story_output_active_;

  public:
    Game();
    Game(Game&) = delete;
    ~Game(){};
    Game(char *dungeon_path, char *config_path);

    void toggleGame();
    bool isRunning() const;
    void toggleStoryOutput();
    void start();
    void doCommand();
    void doCommand(std::string input);
};


#endif //GAME_HPP