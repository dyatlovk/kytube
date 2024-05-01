#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QWidget>

#include "../models/history.hpp"
#include "../models/log.hpp"
#include "../models/search.hpp"
#include "../models/stream.hpp"
#include "../views/ui_main.h"
#include "about.hpp"
#include "history.hpp"
#include "log.hpp"
#include "preferences.hpp"
#include "stream.hpp"

namespace Ui
{
  class MainWindow;
}

namespace ui
{
  class MainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    static auto CloseWindow() -> void;

  private:
    auto ShowVideoMenu(const QPoint &pos) -> void;

    auto OnPageNext() -> void;

    auto OnSearchTrigger() -> void;

    auto OnAbout() -> void;

    auto OnLog() -> void;

    auto OnHistory() -> void;

    auto OnPreferences() -> void;

  protected:
    void closeEvent(QCloseEvent *event) override;

  private:
    auto settingsPresent() -> bool;

    auto forcePreferences() -> void;

  private:
    Ui::MainWindow *main;
    About *about;
    Log *log;
    History *history;
    QSettings *settings;
    Preferences *preferences;
    models::search *videoModel;
    models::log *logModel;
    models::history *historyModel;
    StreamDialog *streamDialog;
    models::stream *streamModel;
  };
} // namespace ui
