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
#include "../story/Story.hpp"
#include "../entity/character/Player.hpp"

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
    inline static Story story_ = Story();
    inline static int max_players_ = 0;
    std::unique_ptr<CommandParser> parser_;
    vector<std::shared_ptr<Player>> players_;
    int action_count_ = 0;
    Dungeon dungeon_;
    Phase current_phase_;
    bool is_running_ = true;
    bool map_output_active_ = true;
    bool story_output_active_ = true;
    bool command_finished_;

  public:
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;
    ~Game(){};
    Game(char *dungeon_path, char *config_path);

    void toggleGameRunning() { is_running_ = !is_running_; }
    void toggleMapOutput() { map_output_active_ = !map_output_active_; }
    void toggleStoryOutput() { story_output_active_ = !story_output_active_; }
    bool isRunning() const;
    void start();

    //Function that promts user input and executes command
    void doCommand();
    //Overloaded doCommand function that executes a command based on a string
    void doCommand(string input);
    bool playerExists(string name);
    int getPlayerTypeAmount(char type);
    std::shared_ptr<Room> getCurrentRoom();
    vector<std::shared_ptr<Player>> getPlayers() { return players_; }
    std::shared_ptr<Player> getPlayerByType(char type);
    void printStoryAndRoom(bool print_story = true);
    void movePlayer(char player_abbrev, std::pair<int, int> position);
    void moveToRoom(int room_id);
    void lootEntity(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity);
};

#endif //GAME_HPP