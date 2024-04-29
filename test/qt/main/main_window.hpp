#pragma once

#include <QMainWindow>
#include <QWidget>
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

  protected:
    void closeEvent(QCloseEvent *event) override;

  private:
    Ui::MainWindow *main;
  };
} // namespace TestQt
