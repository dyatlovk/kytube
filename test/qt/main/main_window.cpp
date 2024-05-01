#include "main_window.hpp"

#include <qpoint.h>
#include <unistd.h>

#include "test/qt/main/Worker.hpp"

namespace TestQt
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
      , streamDialog(new StreamDialog)
      , videoModel(new models::search())
      , worker(new Worker)
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
    delete worker;
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

    connect(worker, &Worker::textReady, this, &MainWindow::updateTextField);

    connect(&infoAction, &QAction::triggered,
        [this]
        {
          worker->startAsyncTask();
          streamDialog->Get()->description->setPlaceholderText("Loading");
          streamDialog->Get()->description->setAlignment(Qt::AlignCenter);
          streamDialog->show();
          connect(streamDialog->Get()->closeButton, &QPushButton::released, [this] { streamDialog->Close(); });
          connect(streamDialog->Get()->playButton, &QPushButton::released, [this]() { streamDialog->Close(); });
        });

    contextMenu.exec(main->videoList->viewport()->mapToGlobal(pos));
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QWidget::closeEvent(event);
    CloseWindow();
  }

  auto MainWindow::updateTextField(const QString &text) -> void
  {
    streamDialog->Get()->uploader->setText("Uploader name");
    streamDialog->Get()->title->setText("Stream title");
    streamDialog->Get()->title->setStyleSheet("font-weight: bold;");
    streamDialog->Get()->description->setHtml(text);
  }
} // namespace TestQt
