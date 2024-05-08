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

  Game game(argv[1], argv[2]);
  game.start();
  //
  //CommandParser cli;
  //while(game.isRunning())
  //{
  //  Command command = cli.readCommand();
  //  if (command.isQuit())
  //    break;
  //  game.executeCommand(command);
  //}

  return 0;
}