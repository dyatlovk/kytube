#pragma once

#include <filesystem>

namespace models
{
  class log
  {
  public:
    struct RowData
    {
      std::string date;
      std::string reason;
      std::string message;
    };

    log();

    ~log();

    auto Append(const RowData &data) -> std::string;

    auto FlushOnDisk() -> void;

    auto RestoreFromDisk() -> void;

    auto GetMessages() -> std::string;

  private:
    std::string data_;
    std::filesystem::path file_;
    std::filesystem::path cfgRoot_;
  };
} // namespace models
