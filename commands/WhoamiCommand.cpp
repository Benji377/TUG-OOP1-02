#include "WhoamiCommand.hpp"


void WhoamiCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::cout << "Team: Q-If-Else-ers" << std::endl;

}