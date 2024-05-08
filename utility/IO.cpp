#include "IO.hpp"

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

/*void IO::printPlayerPositions(std::map<std::shared_ptr<Entity>, std::string> players)
{ OLD VERSION
  for (int idx_player = 1; idx_player <= players.size(); ++idx_player)
  {
    for (const auto& pair : players)
    {
      if (pair.first->getId() == idx_player)
      {
          std::cout << pair.first->getTypeName() << pair.first->getAbbreviation() << pair.first->getName()
            << pair.second << std::endl;
      }
    }
  } 
} 
Ü*/
