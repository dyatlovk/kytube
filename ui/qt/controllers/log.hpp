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

    auto GetUi() -> Ui::Log * { return this->ui; };

    auto Add(const LogFields &field) -> void;

    auto Clear() -> void { m_logs.clear(); };

  private:
    Ui::Log *ui;
    std::vector<LogFields> m_logs;
  };
} // namespace ui
