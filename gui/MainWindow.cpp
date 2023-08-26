#include "MainWindow.h"

#include <QCloseEvent>
#include <QCoreApplication>
#include <QMenu>
#include <ctime>
#include <iostream>

#include "../Config.hpp"
#include "../players/Mpv.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , main(new Ui::MainWindow)
    , log(nullptr)
    , logModel()
    , settings(nullptr)
    , about(nullptr)
    , preferences(nullptr)
    , videoModel(nullptr)
{
  main->setupUi(this);
  logModel = new LogModel;
  settings = new QSettings(std::string(YoutubeQt::DOMAIN_NAME).c_str(), std::string(YoutubeQt::APP_NAME).c_str());

  main->videoList->setContextMenuPolicy(Qt::CustomContextMenu);
  videoModel = new VideoModel();
  main->videoList->setModel(videoModel);
  main->nextPageButton->setDisabled(true);
  main->prevPageButton->setDisabled(true);

  connect(main->actionQuit, &QAction::triggered, this, &MainWindow::CloseWindow);
  connect(main->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);
  connect(main->actionPreferences, &QAction::triggered, this, &MainWindow::OnPreferences);
  connect(main->searchButton, &QPushButton::released, this, &MainWindow::OnSearchTrigger);
  connect(main->videoList, &QTableView::customContextMenuRequested, this, &MainWindow::ShowContentMenu);
  connect(main->actionLog, &QAction::triggered, this, &MainWindow::OnLog);
  connect(main->nextPageButton, &QPushButton::released, this, &MainWindow::onPageNext);
  connect(main->prevPageButton, &QPushButton::released, this, &MainWindow::onPagePrev);
}

MainWindow::~MainWindow()
{
  if (settings)
  {
    delete settings;
  }

  if (videoModel)
  {
    delete videoModel;
  }

  if (preferences)
  {
    delete preferences;
  }

  if (log)
  {
    delete log;
  }

  if (about)
  {
    delete about;
  }

  if (logModel)
  {
    delete logModel;
  }

  if (main)
  {
    delete main;
  }
}

void MainWindow::OnAbout()
{
  const std::string version(YoutubeQt::VERSION);
  const std::string app(YoutubeQt::APP_NAME);
  const std::string aboutText = app + " v." + version;
  about = new AboutDialog();
  about->GetUi()->aboutText->setText(QString(aboutText.c_str()));
  about->Show();
  about->setAttribute(Qt::WA_DeleteOnClose);
  about->setWindowTitle(tr("QTube - About"));
  connect(about->GetUi()->closeButton, &QPushButton::released,
      [this]()
      {
        about->Close();
        delete about;
      });
}

auto MainWindow::OnPreferences() -> void
{
  preferences = new Preferences();
  preferences->show();
  preferences->setWindowTitle(tr("QTube - Preferences"));
  preferences->setAttribute(Qt::WA_DeleteOnClose);
  preferences->GetUi()->apiKey->setText(settings->value("Youtube/apiKey").toString());
  preferences->GetUi()->search->setText(settings->value("Youtube/search").toString());
  preferences->GetUi()->resultsPerPage->setValue(settings->value("Youtube/resultsPerPage").toInt());
  preferences->GetUi()->watchUrl->setText(settings->value("Youtube/watchUrl").toString());
  connect(preferences->GetUi()->cancelButton, &QPushButton::released, [this]() { preferences->close(); });
  connect(preferences->GetUi()->saveButton, &QPushButton::released, [this]() { preferences->SaveSettingsAndClose(); });
  connect(preferences->GetUi()->applyButton, &QPushButton::released, [this]() { preferences->SaveSettings(); });

  auto hostRect = main->centralwidget->geometry();
  auto widgetRect = preferences->geometry();
  preferences->move(hostRect.center().x() - widgetRect.width() / 2, hostRect.center().y() - widgetRect.height() / 2);
}

auto MainWindow::ShowContentMenu(const QPoint &pos) -> void
{
  QModelIndex index = main->videoList->indexAt(pos);
  const auto data = videoModel->FindDataByIndex(index);
  const auto mpv = new Players::Mpv();
  auto watchUrl = settings->value("Youtube/watchUrl").toString().append(data.videoId.c_str());

  QMenu contextMenu(tr("Context menu"), this);

  QAction playAction("Play", this);
  contextMenu.addAction(&playAction);
  connect(
      &playAction, &QAction::triggered, [&data, &mpv, &watchUrl]() { mpv->Handler(watchUrl.toStdString().c_str()); });
  QAction infoAction("Info", this);
  contextMenu.addSeparator();
  contextMenu.addAction(&infoAction);
  auto hostRect = main->centralwidget->geometry();
  auto widgetRect = contextMenu.geometry();
  contextMenu.move(pos.x(), pos.y() + contextMenu.geometry().height());
  contextMenu.exec(mapToGlobal(pos));
  delete mpv;
}

auto MainWindow::OnSearchTrigger() -> void
{
  const auto q = main->searchField->text();
  videoModel->ResetModel();
  const auto apiKey = settings->value("Youtube/apiKey").toString();
  const auto resultsPerPage = settings->value("Youtube/resultsPerPage").toInt();
  const auto searchUrl = settings->value("Youtube/search").toString();
  try
  {
    videoModel->RequestApi(q.toStdString(), resultsPerPage, apiKey.toStdString(), searchUrl.toStdString());
    if (videoModel->getParsedData())
    {
      if (videoModel->getParsedData()->prevPageToken)
      {
        main->prevPageButton->setDisabled(false);
      }

      if (videoModel->getParsedData()->nextPageToken)
      {
        main->nextPageButton->setDisabled(false);
      }
    }
  }
  catch (youtube::Exception &err)
  {
    std::cout << err.what() << std::endl;
    std::time_t now = std::time(nullptr);
    logModel->AppendData({std::ctime(&now), err.what()->reason, err.what()->message});
  }

  main->videoList->resizeColumnsToContents();
}

auto MainWindow::OnLog() -> void
{
  log = new Log();
  log->GetUi()->logTable->setModel(logModel);
  log->setWindowTitle(tr("YouTubeQt - Log"));
  log->show();

  connect(log->GetUi()->closeButton, &QPushButton::released, this,
      [this]()
      {
        log->close();
        delete log;
      });
}

auto MainWindow::onPageNext() -> void
{
  if (!videoModel->getParsedData())
    return;
  const auto pageToken = videoModel->getParsedData()->nextPageToken;
  if (!pageToken)
    return;

  const auto q = videoModel->GetQuery();
  videoModel->ResetModel();
  const auto apiKey = settings->value("Youtube/apiKey").toString();
  const auto resultsPerPage = settings->value("Youtube/resultsPerPage").toInt();
  const auto searchUrl = settings->value("Youtube/search").toString();

  try
  {
    videoModel->RequestApi(q, resultsPerPage, apiKey.toStdString(), searchUrl.toStdString(), *pageToken);
  }
  catch (youtube::Exception &err)
  {
    std::cout << err.what() << std::endl;
    std::time_t now = std::time(nullptr);
    logModel->AppendData({std::ctime(&now), err.what()->reason, err.what()->message});
  }
  main->videoList->resizeColumnsToContents();
}

auto MainWindow::onPagePrev() -> void
{
  if (!videoModel->getParsedData())
    return;
  const auto pageToken = videoModel->getParsedData()->prevPageToken;
  if (!pageToken)
    return;

  const auto q = videoModel->GetQuery();
  videoModel->ResetModel();
  const auto apiKey = settings->value("Youtube/apiKey").toString();
  const auto resultsPerPage = settings->value("Youtube/resultsPerPage").toInt();
  const auto searchUrl = settings->value("Youtube/search").toString();

  try
  {
    videoModel->RequestApi(q, resultsPerPage, apiKey.toStdString(), searchUrl.toStdString(), *pageToken);
  }
  catch (youtube::Exception &err)
  {
    std::cout << err.what() << std::endl;
    std::time_t now = std::time(nullptr);
    logModel->AppendData({std::ctime(&now), err.what()->reason, err.what()->message});
  }
  main->videoList->resizeColumnsToContents();
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
