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
#include <string>
#include <map>
#include <iostream>

// Enum for the different exceptions
enum class Exceptions
{
  MEMORY_ALLOCATION_ERROR = 1,
  WRONG_NUMBER_OF_PARAMETERS = 2,
  INVALID_CONFIG_FILE = 3,
};

class Exception : public std::exception
{
  private:
    std::string message_;
    std::map<Exceptions, std::string> exception_messages_ = {
      {Exceptions::MEMORY_ALLOCATION_ERROR, "Error: Not enough memory!"},
      {Exceptions::WRONG_NUMBER_OF_PARAMETERS, "Error: Wrong number of parameters!"},
      {Exceptions::INVALID_CONFIG_FILE, "Error: Invalid file (<FILE_PATH>)!"}
    };

  public:

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Exception class. Initializes the message with the corresponding message for the given
    /// exception. If the exception is INVALID_CONFIG_FILE, the file path is added to the message.
    /// @param exception The exception that should be thrown.
    /// @param file_path The path to the file that caused the exception.
    //
    Exception(Exceptions exception, std::string file_path = "");

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Exception class. Initializes the message with the given message.
    /// @param message The message that should be thrown.
    //
    Exception(const std::string& message){ message_ = message; }

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the message of the exception.
    /// @return The message of the exception.
    //
    virtual const char* what() const noexcept override { return message_.c_str(); }
};