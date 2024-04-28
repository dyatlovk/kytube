#include "log.hpp"

#include <fstream>
#include <iostream>
#include <qsettings.h>

namespace models
{
  log::log()
      : file_("")
      , cfgRoot_("")
  {
    const auto settings = new QSettings(DOMAIN_NAME, CONFIG_NAME);
    auto settingsFile = settings->fileName();
    cfgRoot_ = std::filesystem::path(settingsFile.toStdString()).parent_path();
    file_ = cfgRoot_.append("app.log");
    delete settings;
    RestoreFromDisk();
  }

  log::~log() {}

  auto log::Append(const RowData &data) -> std::string
  {
    std::string line;
    line.append(data.date + " ");
    line.append(data.reason + " ");
    line.append(data.message + "\n");
    data_.append(line);
    return line;
  }

  auto log::FlushOnDisk() -> void
  {
    std::ofstream log;
    log.open(file_, std::ios_base::ate);
    log << data_;
    log.close();
  }

  auto log::RestoreFromDisk() -> void
  {
    std::string line;
    std::ifstream log;
    log.open(file_);
    data_.clear();
    while (std::getline(log, line))
    {
      data_.append(line + "\n");
    }
    log.close();
  }

  auto log::GetMessages() -> std::string
  {
    return data_;
  }
} // namespace models
