#pragma once

#include <QDialog>

#include "../views/ui_history.h"

namespace Ui
{
  class History;
}

namespace ui
{
  class History : public QDialog
  {
    Q_OBJECT
  public:
    explicit History(QDialog *parent = nullptr);

    ~History() override;

    auto Close() -> void;

    auto Show() -> void;

    auto GetUi() -> Ui::History * { return this->ui; };

  private:
    Ui::History *ui;
  };
} // namespace ui
