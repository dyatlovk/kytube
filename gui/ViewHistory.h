#pragma once

#include <qt5/QtWidgets/qdialog.h>

#include "../forms/ui_views.h"

namespace Ui
{
  class Views;
}

class ViewHistory : public QDialog
{
  Q_OBJECT
public:
  explicit ViewHistory(QDialog *parent = nullptr);

  ~ViewHistory() override;

  auto GetUi() -> Ui::Views * { return this->ui; };

private:
  Ui::Views *ui;
};
