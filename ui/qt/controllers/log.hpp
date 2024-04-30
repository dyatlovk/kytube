#pragma once

#include <QDialog>

#include "../views/ui_logs.h"

namespace Ui
{
  class Log;
}

namespace ui
{
  class Log : public QDialog
  {
    Q_OBJECT

  public:
    struct LogFields
    {
      std::string date;
      std::string reason;
      std::string message;
    } fields;

    explicit Log(QDialog *parent = nullptr);

    ~Log() override;

    auto Close() -> void;

    auto GetUi() -> Ui::Log * { return this->ui; };

    auto hideEvent(QHideEvent *event) -> void override;

    auto OnClose(std::function<void()> callback) -> void;

  private:
    Ui::Log *ui;
    std::function<void()> onClose;
  };
} // namespace ui
