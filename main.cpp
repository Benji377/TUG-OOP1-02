#include "game/Game.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Utils.hpp"

// Remove after testing
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

void print_directory(const fs::path& path, const std::string& indent = "")
{
    std::vector<std::string> ignore_folders = {"build", ".vscode", ".git"};

    if (!fs::exists(path) || !fs::is_directory(path)) return;

    for (const auto& entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.path()) && 
            std::find(ignore_folders.begin(), ignore_folders.end(), entry.path().filename().string()) != ignore_folders.end())
            continue;

        std::cout << indent << entry.path().filename().string() << "\n";

        if (fs::is_directory(entry.path()))
            print_directory(entry.path(), indent + "  ");
    }
}
//


int main(int argc, char** argv)
{
  /* Testing
  fs::path path = fs::current_path();
  print_directory(path);
  */
  //return 0;

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