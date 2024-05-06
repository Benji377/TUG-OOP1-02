#include "Exception.hpp"

Exception::Exception(Exceptions exception, std::string file_path)
{
  if (exception == Exceptions::INVALID_CONFIG_FILE)
  {
    exception_messages_[exception] = "Error: Invalid file (" + file_path + ")!";
  }
  message_ = exception_messages_[exception];
}