#include "game/Game.hpp"
#include "utility/Exception.hpp"
#include "utility/Utils.hpp"


int main(int argc, char** argv)
{

  try {
    if (argc != 3)
    {
      throw Exception(Exceptions::WRONG_NUMBER_OF_PARAMETERS);
    }
    Utils::isValidConfig(argv[1], "DUNGEON");
    Utils::isValidConfig(argv[2], "STORY");
  }
  catch(const Exception& e)
  {
    std::cout << e.what() << std::endl;
    return 2;
  }

  Game game(argv[1], argv[2]);

  return 0;
} 