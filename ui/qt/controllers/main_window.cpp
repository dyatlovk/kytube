#include "main_window.hpp"

#include <filesystem>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <string>

#include "core/datetime/datetime.hpp"
#include "core/network/request.hpp"
#include "core/players/mpv.hpp"

namespace ui
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
      , about(nullptr)
      , history(new History)
      , log(new Log)
      , videoModel(new models::search())
      , logModel(new models::log)
      , historyModel(new models::history)
      , preferences(nullptr)
      , streamDialog(nullptr)
      , settings(new QSettings(DOMAIN_NAME, CONFIG_NAME))
      , streamModel(new models::stream)
  {
    main->setupUi(this);
    main->videoList->setContextMenuPolicy(Qt::CustomContextMenu);
    log->GetUi()->logContent->appendPlainText(
        logModel->Append({core::datetime::Now(), "", "-----------------"}).c_str());
    log->GetUi()->logContent->appendPlainText(
        logModel->Append({core::datetime::Now(), "Info", "App starting"}).c_str());
    main->videoList->setModel(videoModel);

    if (!settingsPresent())
      forcePreferences();

    connect(main->searchButton, &QPushButton::released, this, &MainWindow::OnSearchTrigger);
    connect(main->searchField, &SearchInput::queryEnter, this, &MainWindow::OnSearchTrigger);
    connect(main->actionQuit, &QAction::triggered, this, &MainWindow::CloseWindow);
    connect(main->videoList, &QTableView::customContextMenuRequested, this, &MainWindow::ShowVideoMenu);
    connect(main->nextPageButton, &QPushButton::released, this, &MainWindow::OnPageNext);
    connect(main->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);
    connect(main->actionLog, &QAction::triggered, this, &MainWindow::OnLog);
    connect(main->actionSearch, &QAction::triggered, this, &MainWindow::OnHistory);
    connect(main->actionPreferences, &QAction::triggered, this, &MainWindow::OnPreferences);
  }

  MainWindow::~MainWindow()
  {
    delete streamModel;
    delete settings;
    delete historyModel;
    delete logModel;
    delete log;
    delete videoModel;
    delete history;
    delete main;
  }

  auto MainWindow::OnSearchTrigger() -> void
  {
    const auto q = main->searchField->text();
    if (q.size() == 0)
    {
      unlockSearchUi();
      return;
    }
    lockSearchUi();
    if (q.isEmpty())
      return;
    const auto query = network::request::UrlEncode(q.toStdString());
    std::string url = settings->value("Piped/apiUrl").toString().toStdString() + "/search?q=";
    url.append(query);
    url.append("&filter=videos");
    history->GetUi()->plainTextEdit->setPlainText(historyModel->Append(q.toStdString()).c_str());
    videoModel->ResetModel();
    log->GetUi()->logContent->setPlainText(
        logModel->Append({core::datetime::Now(), "Info", "Searching " + q.toStdString()}).c_str());

    connect(videoModel, &models::search::searchComplete, this,
        [this]()
        {
          const auto foundSize = std::to_string(videoModel->GetParsedData().items.size());
          log->GetUi()->logContent->setPlainText(
              logModel->Append({core::datetime::Now(), "Info", "Found " + foundSize + " items"}).c_str());
          main->videoList->resizeColumnsToContents();
          unlockSearchUi();
        });
    videoModel->SearchAsync(url, q.toStdString());
  }

  auto MainWindow::OnPageNext() -> void
  {
    const auto pageToken = videoModel->GetParsedData().nextpage;
    if (pageToken.empty())
    {
      unlockSearchUi();
      return;
    }

    lockSearchUi();
    std::string url = settings->value("Piped/apiUrl").toString().toStdString() + "/nextpage/search?nextpage=";
    const auto query = network::request::UrlEncode(videoModel->GetQuery());
    if (query.empty())
      return;
    url.append(pageToken);
    url.append("&q=" + query);
    url.append("&filter=videos");
    videoModel->ResetModel();
    log->GetUi()->logContent->setPlainText(
        logModel->Append({core::datetime::Now(), "Info", "Searching " + query}).c_str());

    connect(videoModel, &models::search::searchComplete, this,
        [this]()
        {
          const auto foundSize = std::to_string(videoModel->GetParsedData().items.size());
          log->GetUi()->logContent->setPlainText(
              logModel->Append({core::datetime::Now(), "Info", "Found " + foundSize + " items"}).c_str());
          main->videoList->resizeColumnsToContents();
          unlockSearchUi();
        });

    videoModel->SearchAsync(url, videoModel->GetQuery());
  }

  auto MainWindow::CloseWindow() -> void
  {
    QCoreApplication::quit();
  }

  auto MainWindow::ShowVideoMenu(const QPoint &pos) -> void
  {
    QModelIndex index = main->videoList->indexAt(pos);
    if (!index.isValid())
      return;
    const auto data = videoModel->FindDataByIndex(index);

    QMenu contextMenu(tr("Context menu"), this);
    contextMenu.setMinimumWidth(200);
    QAction playAction("Play", this);
    connect(&playAction, &QAction::triggered,
        [&data, this]()
        {
          const auto mpv = new players::mpv();
          const std::string url = settings->value("Piped/streamUrl").toString().toStdString() + data.url;
          mpv->Handler(url.c_str());
          log->GetUi()->logContent->setPlainText(
              logModel->Append({core::datetime::Now(), "Info", "Running stream" + url}).c_str());
          delete mpv;
        });
    contextMenu.addAction(&playAction);
    QAction infoAction("Stream info", this);
    contextMenu.addSeparator();
    contextMenu.addAction(&infoAction);

    connect(&infoAction, &QAction::triggered,
        [this, &data]()
        {
          std::string streamUrl = settings->value("Piped/apiUrl").toString().toStdString() + "/streams/" + data.videoId;
          streamDialog = new StreamDialog;
          streamDialog->show();

          streamModel->SearchAsync(streamUrl);
          streamDialog->Get()->description->setPlaceholderText("Loading...");
          streamDialog->Get()->description->setAlignment(Qt::AlignCenter);

          connect(streamModel, &models::stream::searchComplete, this,
              [this]()
              {
                const auto apiResponse = streamModel->GetParsed();
                streamDialog->Get()->uploader->setText(apiResponse.uploader.c_str());
                streamDialog->Get()->title->setText(apiResponse.title.c_str());
                streamDialog->Get()->title->setStyleSheet("font-weight: bold;");
                streamDialog->Get()->description->setHtml(apiResponse.description.c_str());
              });

          connect(streamDialog->Get()->closeButton, &QPushButton::released,
              [this]
              {
                streamDialog->Close();
                delete streamDialog;
              });

          connect(streamDialog->Get()->playButton, &QPushButton::released, this,
              [this, data]()
              {
                const auto mpv = new players::mpv();
                const std::string url = settings->value("Piped/streamUrl").toString().toStdString() + data.url;
                mpv->Handler(url.c_str());
                delete mpv;
                streamDialog->Close();
                delete streamDialog;
              });
        });

    contextMenu.exec(main->videoList->viewport()->mapToGlobal(pos));
  }

  auto MainWindow::OnAbout() -> void
  {
    about = new About;
    about->SetContent();
    about->Show();
    connect(about->GetUi()->closeButton, &QPushButton::released,
        [this]()
        {
          about->Close();
          delete about;
        });
  }

  auto MainWindow::OnLog() -> void
  {
    log->setWindowTitle(tr("YouTubeQt - Log"));
    log->GetUi()->logContent->setPlainText(logModel->GetMessages().c_str());
    log->show();

    connect(log->GetUi()->closeButton, &QPushButton::released, this, [this]() { log->hide(); });
    log->OnClose([this]() { logModel->FlushOnDisk(); });
  }

  auto MainWindow::OnHistory() -> void
  {
    history->GetUi()->plainTextEdit->setPlainText(historyModel->Get().c_str());
    history->Show();
    connect(history->GetUi()->closeButton, &QPushButton::released, this, [this]() { history->hide(); });
    history->OnClose([this]() { historyModel->FlushOnDisk(); });
  }

  auto MainWindow::OnPreferences() -> void
  {
    preferences = new Preferences();
    preferences->setAttribute(Qt::WA_DeleteOnClose);
    preferences->GetUi()->apiUrlEdit->setText(settings->value("Piped/apiUrl").toString());
    preferences->GetUi()->streamUrlEdit->setText(settings->value("Piped/streamUrl").toString());
    preferences->show();
    connect(preferences->GetUi()->cancelButton, &QPushButton::released, [this]() { preferences->close(); });
    connect(
        preferences->GetUi()->saveButton, &QPushButton::released, [this]() { preferences->SaveSettingsAndClose(); });
    connect(preferences->GetUi()->applyButton, &QPushButton::released, [this]() { preferences->SaveSettings(); });
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    logModel->Append({core::datetime::Now(), "Info", "App closed"});
    logModel->FlushOnDisk();
    QWidget::closeEvent(event);
    CloseWindow();
  }

  auto MainWindow::forcePreferences() -> void
  {
    preferences = new Preferences();
    preferences->setAttribute(Qt::WA_DeleteOnClose);
    preferences->setAttribute(Qt::WA_AlwaysStackOnTop);
    preferences->setModal(true);
    preferences->GetUi()->cancelButton->hide();
    preferences->Show();
    connect(
        preferences->GetUi()->saveButton, &QPushButton::released, [this]() { preferences->SaveSettingsAndClose(); });
    connect(preferences->GetUi()->applyButton, &QPushButton::released, [this]() { preferences->SaveSettings(); });
  }

  auto MainWindow::settingsPresent() -> bool
  {
    auto settingsFile = settings->fileName();
    auto isExists = std::filesystem::exists(settingsFile.toStdString());
    if (!isExists)
      return false;

    auto apiUrlPresent = settings->value("Piped/apiUrl").isValid();
    auto streamUrlPresent = settings->value("Piped/streamUrl").isValid();
    return apiUrlPresent && streamUrlPresent;
  }

  auto MainWindow::lockSearchUi() -> void
  {
    main->searchFrame->setEnabled(false);
    main->pageNavigator->setEnabled(false);
  }

  auto MainWindow::unlockSearchUi() -> void
  {
    main->searchFrame->setEnabled(true);
    main->pageNavigator->setEnabled(true);
  }
} // namespace ui
