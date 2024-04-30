#pragma once

#include <QMainWindow>
#include <QWidget>

#include "search_model.hpp"
#include "stream.hpp"
#include "ui_main.h"

namespace Ui
{
  class MainWindow;
}

namespace TestQt
{
  class MainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    static auto CloseWindow() -> void;

    auto OnSearchTrigger() -> void;

    auto ShowVideoMenu(const QPoint &pos) -> void;

  protected:
    void closeEvent(QCloseEvent *event) override;

  private:
    Ui::MainWindow *main;
    StreamDialog *streamDialog;
    models::search *videoModel;
  };
} // namespace TestQt
