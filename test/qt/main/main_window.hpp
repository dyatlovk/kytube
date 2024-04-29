#pragma once

#include <QMainWindow>
#include <QWidget>

#include "search_model.hpp"
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
    models::search *videoModel;
  };
} // namespace TestQt
