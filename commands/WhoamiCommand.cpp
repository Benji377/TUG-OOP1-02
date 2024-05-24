#include "WhoamiCommand.hpp"


void WhoamiCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::cout << "Insert Team Name" << std::endl;

}