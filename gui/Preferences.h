#pragma once

#include <QDialog>
#include <QSettings>

#include "../forms/ui_preferences.h"

namespace Ui
{
  class Preferences;
}

class Preferences : public QDialog
{
  Q_OBJECT
public:
  explicit Preferences(QDialog *parent = nullptr);

  ~Preferences() override;

  auto GetUi() -> Ui::Preferences * { return this->ui; };

  auto Close() -> void { this->close(); };

  auto Show() -> void { this->show(); };

  auto SaveSettings() -> void;

  auto SaveSettingsAndClose() -> void;

private:
  Ui::Preferences *ui;
  QSettings *settings;
};
