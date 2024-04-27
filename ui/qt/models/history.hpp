#pragma once

#include <string>

namespace models
{
  class history
  {
  public:
    history() = default;

    ~history() = default;

    auto Get() -> std::string { return data_; }

    auto Clear() -> void { data_.clear(); }

    auto Append(const std::string &text) -> void
    {
      data_.append(text);
      data_.append("\n");
    }

  private:
    std::string data_;
  };
} // namespace models
