//----------------------------------------------------------------------------------------------------------------------
// main.cpp
//
// Group: 068
//----------------------------------------------------------------------------------------------------------------------

#include "game/Game.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Utils.hpp"

//----------------------------------------------------------------------------------------------------------------------
///
/// Main function of the game. It checks if the number of arguments is correct and if the config files are valid. If the
/// arguments are correct and the config files are valid, the game is started and the game loop is executed.
///
/// @param argc number of arguments
/// @param argv array of arguments
///
/// @return 0 if the program was successful
//
int main(int argc, char** argv)
{
  try {
  if (argc != 3)
    {
      throw WrongNumberOfParametersException();
    }
    Utils::isValidConfig(argv[1], "DUNGEON");
    Utils::isValidConfig(argv[2], "STORY");
  }
  catch (WrongNumberOfParametersException& e)
  {
    std::cout << e.what() << std::endl;
    return 2;
  }
  catch (InvalidConfigFileException& e)
  {
    std::cout << e.what() << std::endl;
    return 3;
  }

  try {
    Game game(argv[1], argv[2]);
    game.start();

    while(game.isRunning())
    {
      game.step();
    }
  }
  catch (const std::exception& e) {
    std::cout << e.what() << '\n'; //TODO I removed "<< "Main Error: " here in case they check for std::bad alloc, they want it to be "Error: " is ok?
  }

  return 0;
}