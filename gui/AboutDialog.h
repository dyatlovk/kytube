#pragma once

#include <QDialog>

#include "../forms/ui_about.h"

namespace Ui
{
  class About;
}

class AboutDialog : public QDialog
{
  Q_OBJECT
public:
  explicit AboutDialog(QDialog *parent = nullptr);

  ~AboutDialog() override;

  auto Close() -> void;

  auto Show() -> void;

  auto GetUi() -> Ui::About * { return this->ui; };

private:
  Ui::About *ui;
};
