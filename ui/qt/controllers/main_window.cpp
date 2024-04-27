#include "main_window.hpp"

#include "../Config.hpp"
#include "core/network/request.hpp"
#include "core/players/mpv.hpp"

namespace ui
{
  MainWindow::MainWindow(QWidget *parent)
      : QMainWindow(parent)
      , main(new Ui::MainWindow)
      , about(nullptr)
      , history(new History)
      , log(nullptr)
      , videoModel(nullptr)
      , logModel(nullptr)
      , historyModel(nullptr)
      , preferences(nullptr)
      , settings(nullptr)
  {
    main->setupUi(this);
    main->videoList->setContextMenuPolicy(Qt::CustomContextMenu);
    videoModel = new models::search();
    logModel = new models::log;
    logModel->AppendData({"2024-04-27", "Info", "App starting"});
    historyModel = new models::history;
    main->videoList->setModel(videoModel);
    settings = new QSettings(DOMAIN_NAME, CONFIG_NAME);

    connect(main->searchButton, &QPushButton::released, this, &MainWindow::OnSearchTrigger);
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
    delete settings;
    delete historyModel;
    delete logModel;
    delete videoModel;
    delete history;
    delete main;
  }

  auto MainWindow::OnSearchTrigger() -> void
  {
    const auto q = main->searchField->text();
    if (q.isEmpty())
      return;
    const auto query = network::request::UrlEncode(q.toStdString());
    std::string url = settings->value("Piped/apiUrl").toString().toStdString() + "/search?q=";
    url.append(query);
    url.append("&filter=videos");
    historyModel->Append(q.toStdString());
    history->GetUi()->plainTextEdit->setPlainText(historyModel->Get().c_str());
    videoModel->ResetModel();
    logModel->AppendData({"2024-04-27", "Info", "Searching " + q.toStdString()});
    videoModel->Search(url, q.toStdString());
    main->videoList->resizeColumnsToContents();
  }

  auto MainWindow::OnPageNext() -> void
  {
    const auto pageToken = videoModel->GetParsedData().nextpage;
    if (pageToken.empty())
      return;
    std::string url = settings->value("Piped/apiUrl").toString().toStdString() + "/nextpage/search?nextpage=";
    const auto query = network::request::UrlEncode(videoModel->GetQuery());
    if (query.empty())
      return;
    url.append(pageToken);
    url.append("&q=" + query);
    url.append("&filter=videos");
    videoModel->ResetModel();
    logModel->AppendData({"2024-04-27", "Info", "Searching " + query});
    videoModel->Search(url, videoModel->GetQuery());
    main->videoList->resizeColumnsToContents();
  }

  auto MainWindow::CloseWindow() -> void
  {
    QCoreApplication::quit();
  }

  auto MainWindow::ShowVideoMenu(const QPoint &pos) -> void
  {
    QModelIndex index = main->videoList->indexAt(pos);
    const auto data = videoModel->FindDataByIndex(index);

    const auto mpv = new players::mpv();

    QMenu contextMenu(tr("Context menu"), this);
    QAction playAction("Play", this);
    connect(&playAction, &QAction::triggered,
        [&data, &mpv, this]()
        {
          const std::string url = settings->value("Piped/streamUrl").toString().toStdString() + data.url;
          mpv->Handler(url.c_str());
        });
    contextMenu.addAction(&playAction);
    QAction infoAction("Info", this);
    contextMenu.addSeparator();
    contextMenu.addAction(&infoAction);
    contextMenu.move(pos.x(), pos.y() + contextMenu.geometry().height());
    contextMenu.exec(mapToGlobal(pos));

    delete mpv;
  }

  auto MainWindow::OnAbout() -> void
  {
    const std::string version(YoutubeQt::VERSION);
    const std::string app(CONFIG_NAME);
    const std::string aboutText = app + " v." + version;
    about = new About;
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

  auto MainWindow::OnLog() -> void
  {
    log = new Log();
    log->GetUi()->logTable->setModel(logModel);
    log->setWindowTitle(tr("YouTubeQt - Log"));
    log->setAttribute(Qt::WA_DeleteOnClose);
    log->show();

    connect(log->GetUi()->closeButton, &QPushButton::released, this,
        [this]()
        {
          log->close();
          delete log;
        });
  }

  auto MainWindow::OnHistory() -> void
  {
    history->Show();
    history->GetUi()->plainTextEdit->setPlainText(historyModel->Get().c_str());
    connect(history->GetUi()->closeButton, &QPushButton::released, this, [this]() { history->close(); });
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
    QWidget::closeEvent(event);
    CloseWindow();
  }
} // namespace ui
