#pragma once

#include <QDialog>

#include "ui_stream.h"

namespace Ui
{
  class Stream;
}

namespace TestQt
{
  class StreamDialog : public QDialog
  {
    Q_OBJECT
  public:
    explicit StreamDialog(QDialog *parent = nullptr);

    ~StreamDialog() override;

    auto Get() -> Ui::Stream * { return ui; }

    auto Close() -> void;

  private:
    Ui::Stream *ui;
  };
} // namespace TestQt
