#include "Player.hpp"
#include <iomanip>
#include <utility>
#include "../../utility/Props.hpp"

Player::Player(int id, char abbreviation, std::string name) : Character(id, abbreviation)
{
  name_ = std::move(name);
  switch (abbreviation)
  {
    case 'B':
      type_name_ = "Barbarian";
      resistant_to_ = Effect::NONE;
      base_armor_ = 2;
      strength_ = 4;
      vitality_ = 1;
      maximum_health_ = 12 + vitality_;
      health_ = maximum_health_;
      break;
    case 'R':
      type_name_ = "Rogue";
      resistant_to_ = Effect::NONE;
      base_armor_ = 1;
      strength_ = 2;
      vitality_ = 3;
      maximum_health_ = 8 + vitality_;
      health_ = maximum_health_;
      break;
    case 'W':
      type_name_ = "Wizard";
      resistant_to_ = Effect::NONE;
      base_armor_ = 0;
      strength_ = 1;
      vitality_ = 4;
      maximum_health_ = 6 + vitality_;
      health_ = maximum_health_;
      break;
    default:
      throw std::invalid_argument("Invalid abbreviation for player character.");
  }
  initializeInventory();
}

void Player::initializeInventory()
{
  std::shared_ptr<Inventory> temp_inv = std::make_shared<Inventory>(); // Create a shared pointer to Inventory

  if (abbreviation_ == 'B')
  {
    weapon_ = Props::craftWeapon("GAXE", strength_, vitality_);
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("HAXE", strength_, vitality_));
    temp_inv->addWeapon(Props::craftWeapon("HAXE", strength_, vitality_));
  }
  else if (abbreviation_ == 'R')
  {
    weapon_ = Props::craftWeapon("RAPI", strength_, vitality_);
    armor_ = Props::craftArmor("LARM", vitality_);

    temp_inv->addArmor(armor_);
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("DAGG", strength_, vitality_));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", strength_, vitality_));
    temp_inv->addWeapon(Props::craftWeapon("SBOW", strength_, vitality_));
    temp_inv->addAmmunition(Props::craftAmmunition("ARRW", 20));
  }
  else if (abbreviation_ == 'W')
  {
    weapon_ = Props::craftWeapon("QFRC", 'W');
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("QACD", 'W'));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", strength_, vitality_));
  }

  inventory_ = temp_inv;
}

// Getter and setter methods

void Player::setResistance(Effect effect)
{
  resistant_to_ = effect;
}

Effect Player::getResistance() const
{
  return resistant_to_;
}

void Player::setActiveWeapon(std::string weapon_abbreviation)
{
  std::shared_ptr<Weapon> weapon = inventory_->getWeapon(weapon_abbreviation);
  if (weapon != nullptr)
  {
    weapon_ = inventory_->getWeapon(weapon_abbreviation);
  }
  else
  {
    std::cout << "Weapon " << weapon_abbreviation << " not found in inventory.\n";
  }
}

void Player::setArmor(std::string armor_abbreviation)
{
  std::shared_ptr<Armor> armor = inventory_->getArmor(armor_abbreviation);
  if (armor != nullptr)
  {
    armor_ = inventory_->getArmor(armor_abbreviation);
  }
  else
  {
    std::cout << "Armor " << armor_abbreviation << " not found in inventory.\n";
  }
}

std::shared_ptr<Weapon> Player::getActiveWeapon() const
{
  return weapon_;
}


int Player::usePotion(std::string abbreviation)
{
  std::shared_ptr<Potion> potion = inventory_->getPotion(abbreviation);
  if (potion == nullptr)
  {
    return 1;
  }
  else
  {
    if (potion->getEffect() == Effect::HEALTH)
    {
      health_ += potion->getDice()->roll();
      if (health_ > maximum_health_)
      {
        health_ = maximum_health_;
      }
    }
    else
    {
      setResistance(potion->getEffect());
    }
    inventory_->removeItem(potion);
  }
}

// Sorry muss san um di errors zi fixen
void Player::attack(Character& target, int damage) { }

void Player::takeDamage(int damage)
{
  int defense_points = base_armor_ + armor_->getArmorValue();
  int damage_taken = damage - defense_points;
  if (damage_taken < 0)
  {
    damage_taken = 0;
  }
  health_ -= damage_taken;
}
int Player::move(int row, int column) { return 0; }


void Player::printPlayer(const std::pair<std::string, std::string>& position) const
{
  std::cout << type_name_ << " [" << abbreviation_ << "] \"" << name_
            << "\" on (" << position.first << "," << position.second << ")\n"
            << std::setw(15) << std::right << "Armor Value: " << std::setw(5) << std::right << base_armor_ << "\n"
            << std::setw(15) << std::right << "Current Health: " << std::setw(5) << std::right << health_ << "\n"
            << std::setw(15) << std::right << "Max Health: " << std::setw(5) << std::right << maximum_health_ << "\n"
            << std::setw(15) << std::right << "Strength: " << std::setw(5) << std::right << strength_ << "\n"
            << std::setw(15) << std::right << "Vitality: " << std::setw(5) << std::right << vitality_ << "\n";
}

void Player::simplePrint() const
{
  std::cout << "Player " << id_ << ": " << type_name_ << " [" << abbreviation_ << "] \"" << name_ << "\"\n";
}
