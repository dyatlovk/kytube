#include "main_window.hpp"

namespace TestQt
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
  {
    main->setupUi(this);
    connect(main->actionQuit, &QAction::triggered, this, &MainWindow::CloseWindow);
  }

  MainWindow::~MainWindow()
  {
    delete main;
  }

  auto MainWindow::CloseWindow() -> void
  {
    QCoreApplication::quit();
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QWidget::closeEvent(event);
    CloseWindow();
  }
} // namespace TestQt
