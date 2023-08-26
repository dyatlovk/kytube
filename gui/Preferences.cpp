#include "Preferences.h"

#include "../Config.hpp"

Preferences::Preferences(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::Preferences)
    , settings(new QSettings(std::string(YoutubeQt::DOMAIN_NAME).c_str(), std::string(YoutubeQt::APP_NAME).c_str()))
{
  ui->setupUi(this);
}

Preferences::~Preferences()
{
  delete settings;
  delete ui;
}

auto Preferences::SaveSettings() -> void
{
  settings->beginGroup("Youtube");
  settings->setValue("apiKey", ui->apiKey->text());
  settings->setValue("search", ui->search->text());
  settings->setValue("resultsPerPage", GetUi()->resultsPerPage->value());
  settings->endGroup();
}

auto Preferences::SaveSettingsAndClose() -> void
{
  SaveSettings();
  this->close();
}
