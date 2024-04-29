#include "preferences.hpp"

namespace ui
{
  Preferences::Preferences(QDialog *parent)
      : QDialog(parent)
      , ui(new Ui::Preferences)
      , settings(new QSettings(DOMAIN_NAME, CONFIG_NAME))
  {
    ui->setupUi(this);
  }

  Preferences::~Preferences()
  {
    delete ui;
  }

  auto Preferences::SaveSettingsAndClose() -> void
  {
    SaveSettings();
    this->close();
  }

  auto Preferences::SaveSettings() -> void
  {
    settings->beginGroup("Piped");
    settings->setValue("apiUrl", ui->apiUrlEdit->text());
    settings->setValue("streamUrl", ui->streamUrlEdit->text());
    settings->endGroup();
  }

} // namespace ui
