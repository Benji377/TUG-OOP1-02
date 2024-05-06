#include "game/Game.hpp"

int main()
{
Game game;
game.start();

game.doCommand("help");

return 0;
}


/*int main(int argc, char** argv)
{
  if(argc != 3)
  {
    std::cout << "Error: Wrong number of parameters!\n" << std::endl;
    return 2;
  }

  Game game;

  try
  {
    game.parseConfigs(argv[1], argv[2]);
  }
  catch(const std::runtime_error& e)
  {
    std::cout << e.what() << std::endl;
    return 3;
  }




  return 0;

} */