#include "game/Game.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Utils.hpp"


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
  } catch (const std::exception& e) {
    std::cout << "Main Error: " << e.what() << '\n';
  }

  return 0;
}