#include <iostream>

#ifndef DICE_HPP
#define DICE_HPP

/**
 * A simple class to represent a dice with a given number of sides and the amount of times it should be rolled.
 * Furthermore it also has a function to parse a string into a dice. The string is usually like this:
 * "2d6" which means 2 dice with 6 sides each.
 */
class Dice {
public:
    Dice(int sides);
    int roll();
private:
    int sides;
    int dices;
    void parse(std::string dice);
};


#endif //DICE_HPP
