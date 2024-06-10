//---------------------------------------------------------------------------------------------------------------------
//
// The Game class is the main class of the game. It contains the main game loop and the game state. It also contains the
// dungeon, the story, the players and the enemies. The start function gets called once at the beginning of the game and
// the step function gets called every game loop iteration.
//
// Group: 068
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "../dungeon/Dungeon.hpp"
#include "../commands/CommandParser.hpp"
#include "../utility/Props.hpp"
#include "../utility/InputOutput.hpp"
#include "../story/Story.hpp"
#include "../entity/character/Player.hpp"
#include "../entity/character/Enemy.hpp"

using std::vector;
using std::shared_ptr;

const int MIN_PLAYERS = 1;
const int MAX_PLAYERS = 3;
const int MAX_NAME_LENGTH = 10;

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
    vector<shared_ptr<Player>> players_;
    int total_actions_ = 0;
    int action_count_ = 0;
    Dungeon dungeon_;
    Phase current_phase_;
    bool is_running_ = true;
    bool map_output_active_ = true;
    bool story_output_active_ = true;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Copy constructor and assignment operator are deleted because the Game class is a singleton. The destructor is
    /// defaulted.
    //
    Game(Game&) = delete;
    Game& operator=(Game&) = delete;
    ~Game() = default;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Game class
    /// @param dungeon_path the path to the file containing the dungeon layout
    /// @param config_path the path to the file containing the story
    //
    Game(char *dungeon_path, char *config_path);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the dungeon
    ///
    /// @return the dungeon
    //
    Dungeon& getDungeon() { return dungeon_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the story
    ///
    /// @return the story
    //
    Story& getStory() const { return story_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the players
    ///
    /// @return the players
    //
    vector<shared_ptr<Player>> getPlayers() const { return players_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the living players
    ///
    /// @return the living players
    //
    vector<shared_ptr<Player>> getLivingPlayers() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns a player by the type
    ///
    /// @param type the type of the player
    ///
    /// @return the player with the given type
    //
    shared_ptr<Player> getPlayerByType(char type) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the amount of players with the given type
    ///
    /// @param type the type of the player
    ///
    /// @return the amount of players with the given type
    //
    int getPlayerTypeAmount(char type) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks if a player with the given name exists
    ///
    /// @param name the name of the player
    ///
    /// @return true if a player with the given name exists, false otherwise
    //
    bool playerExists(string name) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the max player count
    ///
    /// @return the max player count
    //
    int getMaxPlayerCount() const { return max_players_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the current room
    ///
    /// @return the current room
    //
    shared_ptr<Room> getCurrentRoom() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the action count
    ///
    /// @return the action count
    //
    int getActionCount() const { return action_count_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the map output state
    ///
    /// @return the map output state
    //
    bool getMapOutPutState() { return map_output_active_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the game is running
    ///
    /// @return true if the game is running, false otherwise
    //
    bool isRunning() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether all players are dead
    ///
    /// @return true if all players are dead, false otherwise
    //
    bool allPlayersAreDead() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for the action count
    ///
    /// @param new_count the new action count
    //
    void setActionCount(int new_count) { action_count_ = new_count; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Adds one to the action count
    //
    void plusOneActionCount();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Toggle the game running state
    //
    void toggleGameRunning() { is_running_ = !is_running_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Toggle the map output state
    //
    void toggleMapOutput() { map_output_active_ = !map_output_active_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Toggle the story output state
    //
    void toggleStoryOutput() { story_output_active_ = !story_output_active_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Starts the game
    //
    void start();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Makes a game step
    //
    void step();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Executes a command
    //
    void doCommand();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Executes a command with the given input
    ///
    /// @param input the input string representing the command to be executed
    //
    void doCommand(string input);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the game state
    ///
    /// @param print_story whether the story should be printed
    /// @param print_room_completed whether the room should be printed
    /// @param print_enemy_health whether the enemy health should be printed
    //
    void printStoryAndRoom(bool print_story = true, bool print_room_completed = true, bool print_enemy_health = true);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Prints and saves the score
    //
    void printAndSaveScore();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the score output
    ///
    /// @param os the output stream
    ///
    /// @return the output stream
    //
    std::ostream& returnScoreOutput(std::ostream& os);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Executes the enemy phase
    //
    void enemyPhase();

  bool isLowestPlayer(shared_ptr<Player> player) const;




};

#endif //GAME_HPP