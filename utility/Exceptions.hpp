//----------------------------------------------------------------------------------------------------------------------
//
// The Exception class is a custom exception class that can be used to throw exceptions with custom messages. It has a
// constructor that takes an enum value as a parameter and sets the message accordingly. The class also has a constructor
// that takes a string as a parameter and sets the message accordingly.
//
// Group: 068
//
// Author: Elija Innerkofler, 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <string>
#include <map>
#include <iostream>

#define MEMORY_ALLOCATION_EXCEPTION "Error: Not enough memory!"
#define WRONG_NUMBER_OF_PARAMETERS_EXCEPTION "Error: Wrong number of parameters!"
#define THROWN_QUIT_EXCEPTION "QUIT"


class MemoryAllocationException : public std::exception
{
  public:
    virtual const char* what() const noexcept override { return MEMORY_ALLOCATION_EXCEPTION; }
};

class WrongNumberOfParametersException : public std::exception
{
  public:
    virtual const char* what() const noexcept override { return WRONG_NUMBER_OF_PARAMETERS_EXCEPTION; }
};

class InvalidConfigFileException : public std::exception
{
  private:
    std::string file_path_;
    std::string error_message_;

  public:
    InvalidConfigFileException(std::string file_path) : file_path_(file_path),
      error_message_("Error: Invalid file (" + file_path_ + ")!") {}

    virtual const char* what() const noexcept override { return error_message_.c_str(); }
};


class UnknownCommand : public std::exception
{};

class InvalidParamCommand : public std::exception
{};

class UnavailableItemOrEntityCommand : public std::exception
{};

class InvalidPositionCommand : public std::exception
{};

class CommandExecutionException : public std::exception {
public:
    enum class ExceptionType {
        LOCKED_DOOR, NO_WEAPON_EQUIPPED
    };

private:
  ExceptionType type_;

public:
    CommandExecutionException(ExceptionType type) : type_(type) {};
    ExceptionType getType() const {return type_;};
};


#endif