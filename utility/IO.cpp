#include "IO.hpp"
#include "../entity/character/Player.hpp"
#include "../entity/Entity.hpp"
#include "../dungeon/Room.hpp"

bool IO::checkMagicNumber(char* config_path, std::string magic_number)
{
  std::string filename{config_path};
  std::ifstream file(filename);
  if (!file.is_open())
  {
    return false;
  }

  std::string first_line_of_file;
  std::getline(file, first_line_of_file);

  if(first_line_of_file == magic_number)
  {
    return true;
  }

  return false;
}

std::string IO::promtUserInput()
{
  std::cout << "> " << std::flush;

  std::string input;
  std::getline(std::cin, input);

  //Normalise the input, so you can test for case sensitive QuITs
  std::string input_normalised = input;
  Utils::normalizeString(input_normalised);

  // Check if input is Ctrl + D (end of input)
  if(std::cin.eof() || input_normalised == "quit")
  {
    input = "quit";
  }

  return input;

}

std::vector<std::string> IO::commandifyString(std::string input)
{
  Utils::normalizeString(input);
  std::vector<std::string> vectorised_string = Utils::splitString(input, " ");

  return vectorised_string;
}

void IO::printPlayerPosition(std::shared_ptr<Player> player, std::shared_ptr<Room> room)
{
  std::cout << player->getTypeName() << player->getAbbreviation() << player->getName()
    << room->getFieldOfEntity(player).first << "," << room->getFieldOfEntity(player).first << std::endl;
}


void IO::printEnemyPosition(std::map<std::string, std::shared_ptr<Character>> enemies_mapped,
  std::shared_ptr<Room>  current_room)
{
  for(auto& enemy : enemies_mapped)
  {
    std::cout << enemy.first << current_room->getFieldOfEntity(enemy.second).first
      << current_room->getFieldOfEntity(enemy.second).second;
  }
}

int IO::getDigitAmount(int number)
{
  return std::to_string(number).length();
}

void IO::printPlayerStats(std::shared_ptr<Player> player_ptr)
{
  int armor_val = player_ptr->getFullArmorValue();
  std::cout << "Armor Value:    " << getDigitAmount(armor_val) * " " << armor_val "\n"; // TODO create fun that prints int empty spaces...

  player_ptr->getHealth();
  player_ptr->getMaximumHealth();
  player_ptr->getStrength();
  player_ptr->getVitality();



  std::cout << "Current Health: XXXXX\n";
  std::cout<< "Max Health:     XXXXX\n";
  std::cout << "Strength:       XXXXX\n";
  std::cout << "Vitality:       XXXXX\n";

}