#pragma once

#include <filesystem>
#include <string>

namespace models
{
  class history
  {
  public:
    history();

    ~history();

    auto Get() -> std::string { return data_; }

    auto Clear() -> void { data_.clear(); }

    auto Append(const std::string &text) -> std::string;

    auto FlushOnDisk() -> void;

    auto RestoreFromDisk() -> void;

  private:
    std::string data_;
    std::filesystem::path file_;
    std::filesystem::path cfgRoot_;
  };
} // namespace models
