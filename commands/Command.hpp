#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>

class Game;

class Command
{
  //Only accessible from derived glasses
  protected:
    Command() = default;
  public:
    virtual bool execute() = 0;
    virtual ~Command() {}

};

class HelpCommand: public Command
{
  public:
    HelpCommand() : Command() {};
    virtual bool execute() override;
};

class QuitCommand : public Command
{
  private:
    Game* game_;
  public:
    QuitCommand(Game* game) : Command(), game_(game) {}
    virtual bool execute() override;
    virtual ~QuitCommand() {}
};


#endif
