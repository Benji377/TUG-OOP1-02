#include "Player.hpp"

#include <utility>
#include "../../utility/Props.hpp"

Player::Player(int id, char abbreviation, std::string name) : Character(id, abbreviation)
{
  name_ = std::move(name);
  switch (abbreviation)
  {
    case 'B':
      resistant_to_ = Effect::NONE;
      base_armor_ = 2;
      strength_ = 4;
      vitality_ = 1;
      maximum_health_ = 12 + vitality_;
      health_ = maximum_health_;
      break;
    case 'R':
      resistant_to_ = Effect::NONE;
      base_armor_ = 1;
      strength_ = 2;
      vitality_ = 3;
      maximum_health_ = 8 + vitality_;
      health_ = maximum_health_;
      break;
    case 'W':
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

void Player::initializeInventory() {
  Inventory temp_inv = *new Inventory();
  if (abbreviation_ == 'B')
  {
    weapon_ = Props::craftWeapon((std::string &) "GAXE", strength_, vitality_);
    armor_ = nullptr;
    temp_inv.addWeapon(weapon_);
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "HAXE", strength_, vitality_));
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "HAXE", strength_, vitality_));
  }
  else if (abbreviation_ == 'R')
  {
    weapon_ = Props::craftWeapon((std::string &) "RAPI", strength_, vitality_);
    armor_ = Props::craftArmor((std::string &) "LARM", vitality_);

    temp_inv.addArmor(armor_);
    temp_inv.addWeapon(weapon_);
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "DAGG", strength_, vitality_));
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "DAGG", strength_, vitality_));
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "SBOW", strength_, vitality_));
    temp_inv.addAmmunition(Props::craftAmmunition((std::string &) "ARRW", 20));
  }
  else if (abbreviation_ == 'W')
  {
    weapon_ = Props::craftWeapon((std::string &) "QFRC", 'W');
    armor_ = nullptr;
    temp_inv.addWeapon(weapon_);
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "QACD", 'W'));
    temp_inv.addWeapon(Props::craftWeapon((std::string &) "DAGG", strength_, vitality_));
  }
  inventory_ = &temp_inv;
}


// Sorry muss san um di errors zi fixen
void Player::attack(Character& target, int damage) { }
void Player::take_damage(int damage) { }
int Player::move(int row, int column) { return 0; }

Player::~Player()
{
  delete inventory_.value();
  delete weapon_;
  delete armor_;
}