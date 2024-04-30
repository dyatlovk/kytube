#include "main_window.hpp"

#include <qpoint.h>

namespace TestQt
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
      , streamDialog(nullptr)
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
    delete streamDialog;
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

    connect(&infoAction, &QAction::triggered,
        [this]
        {
          streamDialog = new StreamDialog;
          streamDialog->Get()->uploader->setText("Uploader name");
          streamDialog->Get()->title->setText("Stream title");
          streamDialog->Get()->title->setStyleSheet("font-weight: bold;");
          streamDialog->Get()->description->setHtml("description");
          streamDialog->show();
          connect(streamDialog->Get()->closeButton, &QPushButton::released,
              [this]
              {
                streamDialog->Close();
                delete streamDialog;
              });
        });

    contextMenu.exec(main->videoList->viewport()->mapToGlobal(pos));
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QWidget::closeEvent(event);
    CloseWindow();
  }
} // namespace TestQt
