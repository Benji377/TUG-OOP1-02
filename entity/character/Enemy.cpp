#include "Enemy.hpp"

Enemy::Enemy(int id, char abbreviation): Character(id, abbreviation) {}


// Sorry muss san um di errors zi fixen
void Enemy::attack(Character& target, int damage) { }
void Enemy::take_damage(int damage) { }
int Enemy::move(int row, int column) { return 0; }