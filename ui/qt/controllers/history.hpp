#pragma once

#include <QDialog>
#include <functional>

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

    auto hideEvent(QHideEvent *event) -> void override;

    auto OnClose(std::function<void()> callback) -> void;

  private:
    Ui::History *ui;
    std::function<void()> onClose;
  };
} // namespace ui
