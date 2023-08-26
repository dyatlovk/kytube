#pragma once

#include <QDialog>
#include <vector>

#include "../forms/ui_log.h"

namespace Ui
{
  class Log;
}

class Log : public QDialog
{
  struct LogFields
  {
    std::string date;
    std::string reason;
    std::string message;
  } fields;

  Q_OBJECT
public:
  explicit Log(QDialog *parent = nullptr);
  ~Log() override;

  auto GetUi() -> Ui::Log * { return this->ui; };

  auto Add(const LogFields &field) -> void;

  auto Clear() -> void { m_logs.clear(); };

private:
  Ui::Log *ui;
  std::vector<LogFields> m_logs;
};
