#include "history.hpp"

#include <fstream>
#include <iostream>
#include <qsettings.h>

namespace models
{
  history::history()
      : file_("")
      , cfgRoot_("")
  {
    const auto settings = new QSettings(DOMAIN_NAME, CONFIG_NAME);
    auto settingsFile = settings->fileName();
    cfgRoot_ = std::filesystem::path(settingsFile.toStdString()).parent_path();
    file_ = cfgRoot_.append("history.log");
    delete settings;
    RestoreFromDisk();
  }

  history::~history() {}

  auto history::Append(const std::string &text) -> std::string
  {
    data_.append(text + "\n");
    return text;
  }

  auto history::FlushOnDisk() -> void
  {
    std::ofstream log;
    log.open(file_, std::ios_base::ate);
    log << data_;
    log.close();
  }

  auto history::RestoreFromDisk() -> void
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

} // namespace models
