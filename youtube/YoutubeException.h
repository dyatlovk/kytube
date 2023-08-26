#pragma once
#include <exception>
#include <string>

namespace youtube
{
  struct Error
  {
    int code;
    std::string message;
    std::string reason;
  };
  class Exception : public std::exception
  {
  public:
    Exception(const char *msg, const char *reason, int code)
        : _msg(msg)
        , _reason(reason)
        , code(code)
    {
      error.code = code;
      error.message = msg;
      error.reason = reason;
    };

    ~Exception() override{};

    auto what() -> Error * { return &error; };

    Error error;

  private:
    const char *_msg;
    const char *_reason;
    int code;
  };
} // namespace youtube
