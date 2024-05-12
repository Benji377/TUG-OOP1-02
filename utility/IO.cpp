#include "IO.hpp"
#include "../entity/character/Player.hpp"
#include "../entity/Entity.hpp"
#include "../dungeon/Room.hpp"
#include "../entity/character/Inventory.hpp"
#include "../items/Item.hpp"

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

std::string IO::promtUserInput(bool check_input)
{
  std::cout << "> " << std::flush;

  std::string input;
  std::getline(std::cin, input);

  //Normalise the input, so you can test for case sensitive QuITs
  std::string input_normalised = input;
  Utils::normalizeString(input_normalised);

  // Check if input is Ctrl + D (end of input)
  if(check_input && (std::cin.eof() || input_normalised == "quit"))
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
  std::cout << player->getTypeName() << " ";
  std::cout << "[" << player->getAbbreviation() << "]" << " ";
  std::cout << "\"" << player->getName() << "\"" << " ";
  std::pair<int, int> field = room->getFieldOfEntity(player);
  std::cout << "on (" << field.first << "," << field.second << ")" << std::endl;
}


void IO::printEnemyPosition(std::map<std::string, std::shared_ptr<Character>> enemies_mapped,
  std::shared_ptr<Room>  current_room)
{
  for(auto& enemy : enemies_mapped)
  {
    std::pair<int, int> field = current_room->getFieldOfEntity(enemy.second);
    std::cout << enemy.first << " [" << enemy.second->getAbbreviation();
    std::cout << enemy.second->getId() << "] on (" << field.first;
    std::cout << ","  << field.second << ")" << std::endl;
  }
}

void IO::printVectorOfItemsAlphabetically(std::vector<std::shared_ptr<Item>> items)
{
  std::map<std::string, int> item_strings_sorted_with_map;

  for(const auto& item : items)
  {
    std::string item_string = " [" + item->getAbbreviation() + "] " + item->getName();

    if(item_strings_sorted_with_map.count(item_string) == 0)
    {
      item_strings_sorted_with_map.insert(std::make_pair(item_string, 1));
    }
    else
    {
      item_strings_sorted_with_map.at(item_string)++;
    }
  }

for(auto item_string = item_strings_sorted_with_map.begin(); item_string != item_strings_sorted_with_map.end(); ++item_string)
{
  std::cout << item_string->first << " (" << item_string->second << ")";
  if (std::next(item_string) != item_strings_sorted_with_map.end()) 
  {
    // Not the end, print a delimiter (e.g., comma)
    std::cout << ", ";
  }
  else
  {
    std::cout << std::endl;
  }
}


}


void IO::printInventory(std::shared_ptr<Player> player)
{
  std::cout << "Inventory " << "\"" << player->getName() << "\"" << std::endl;
  std::cout << "  Equipped Armor: "; 

  if(player->getArmor() != nullptr)
  {
    std::cout << "[" << player->getArmor()->getAbbreviation() << "] " <<  player->getArmor()->getName() << std::endl;
  }
  else
  {
    std::cout << "[-] None" << std::endl;
  }

  std::cout << "  Equipped Weapon: "; 

  if(player->getActiveWeapon() != nullptr)
  {
    std::cout << "[" << player->getActiveWeapon()->getAbbreviation() << "] " <<  player->getActiveWeapon()->getName() << std::endl;
  }
  else
  {
    std::cout << "[-] None" << std::endl;
  }

  std::shared_ptr<Inventory> inv = player->getInventory();

  //TODO can part of Inventory be empty? What then?
  std::cout << "  Armor:";

  std::vector<std::shared_ptr<Armor>> armor_ptrs = inv->getAllArmor();
  //TODO remove equipped armor here
  std::vector<std::shared_ptr<Item>> armor_as_items;
  for(const auto& armor_ptr : armor_ptrs) 
  {
    //Code line from GeminiAI to perform a dynamic pointer cast
    //Begin
    std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(armor_ptr);
    //end
    if(item != nullptr) 
    {
      armor_as_items.push_back(item);
    }
  }
  printVectorOfItemsAlphabetically(armor_as_items);

  std::cout << "  Weapons:";

  std::vector<std::shared_ptr<Weapon>> weapon_ptrs = inv->getAllWeapons();
  //TODO remove equipped weapon here
  std::vector<std::shared_ptr<Item>> weapons_as_items;
  for(const auto& weapon_ptr : weapon_ptrs) 
  {
    std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(weapon_ptr);
    if(item != nullptr) 
    {
      weapons_as_items.push_back(item);
    }
  }

  printVectorOfItemsAlphabetically(weapons_as_items);

/*
  std::cout << "  Consumables:";

  std::vector<std::shared_ptr<Weapon>> weapon_ptrs = inv->getAllWeapons();
  std::vector<std::shared_ptr<Item>> weapons_as_items;
  for(const auto& weapon_ptr : weapon_ptrs) 
  {
    std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(weapon_ptr);
    if(item != nullptr) 
    {
      weapons_as_items.push_back(item);
    }
  }

  printVectorOfItemsAlphabetically(weapons_as_items);
*/
}