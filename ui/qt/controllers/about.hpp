#pragma once

#include <QDialog>

#include "../views/ui_about.h"

namespace Ui
{
  class About;
}

namespace ui
{
  class About : public QDialog
  {
    Q_OBJECT
  public:
    explicit About(QDialog *parent = nullptr);

    ~About() override;

    auto Close() -> void;

    auto Show() -> void;

    auto GetUi() -> Ui::About * { return this->ui; };

  private:
    Ui::About *ui;
  };
} // namespace ui
