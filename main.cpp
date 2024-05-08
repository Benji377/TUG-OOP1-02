#include <csignal>
#include <filesystem>
#include "game/Game.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Utils.hpp"


int main(int argc, char** argv)
{
  /* Testing
  fs::path path = fs::current_path();
  print_directory(path);
  */
  //return 0;

  // We need to inspect some issues with the CSV parser and path finding
  // Therefore we need to check the current path and the current working directory
  // Using nothing but standard C++17
  std::cout << "Current path using getcwd(): " << getcwd(nullptr, 0) << std::endl;
  std::cout << "Current path using realpath(): " << realpath(".", nullptr) << std::endl;

  // Now we want to check which folder exists in the same directory as the executable
  // We will check for the existence of the "data" folder. The Utils class does not have any functions for this
  // We will use the filesystem library to list all files and folders in the current directory
  std::cout << "Files and folders in the current directory: " << std::endl;
  for (const auto& entry : std::filesystem::directory_iterator("."))
  {
    std::cout << entry.path() << std::endl;
  }
  // Now we check if the folder items exist
  std::cout << "Checking if the 'items' folder exists: " << std::endl;
  if (std::filesystem::exists("items"))
  {
    std::cout << "The 'items' folder exists" << std::endl;
  }
  else
  {
    std::cout << "The 'items' folder does not exist" << std::endl;
  }
  
  std::cout << "Termination of current path check" << std::endl;

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