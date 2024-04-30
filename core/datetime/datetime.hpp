#pragma once

#include <string>

namespace core
{
  class datetime
  {
  public:
    datetime();

    ~datetime();

    static auto Now(const std::string &format = "%Y-%m-%d %H:%M:%S") -> const std::string;
  };
} // namespace core
