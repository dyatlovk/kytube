#include "main_window.hpp"
#include <qpoint.h>

namespace TestQt
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
      , videoModel(new models::search())
  {
    main->setupUi(this);
    main->videoList->setContextMenuPolicy(Qt::CustomContextMenu);

    main->videoList->setModel(videoModel);

    connect(main->videoList, &QTableView::customContextMenuRequested, this, &MainWindow::ShowVideoMenu);
    connect(main->searchButton, &QPushButton::released, this, &MainWindow::OnSearchTrigger);
    connect(main->actionQuit, &QAction::triggered, this, &MainWindow::CloseWindow);
  }

  MainWindow::~MainWindow()
  {
    delete videoModel;
    delete main;
  }

  auto MainWindow::OnSearchTrigger() -> void
  {
    videoModel->ResetModel();
    videoModel->Search("fake_url", "query");
    main->videoList->resizeColumnsToContents();
  }

  auto MainWindow::CloseWindow() -> void
  {
    QCoreApplication::quit();
  }

  auto MainWindow::ShowVideoMenu(const QPoint &pos) -> void
  {
    QMenu contextMenu(tr("Context menu"), this);
    contextMenu.setMinimumWidth(200);
    QAction playAction("Play", this);
    contextMenu.addAction(&playAction);
    QAction infoAction("Info", this);
    contextMenu.addSeparator();
    contextMenu.addAction(&infoAction);
    contextMenu.exec(main->videoList->viewport()->mapToGlobal(pos));
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QWidget::closeEvent(event);
    CloseWindow();
  }
} // namespace TestQt
