#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QWidget>

#include "../forms/ui_main.h"
#include "../model/VideoModel.h"
#include "../model/LogModel.h"
#include "AboutDialog.h"
#include "Log.h"
#include "Preferences.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  auto OnAbout() -> void;

  auto OnPreferences() -> void;

  auto OnSearchTrigger() -> void;

  auto OnLog() -> void;

  auto ShowContentMenu(const QPoint &pos) -> void;

  static auto CloseWindow() -> void;

  auto onPageNext() -> void;

  auto onPagePrev() -> void;

  Ui::MainWindow *main;
  Log *log;
  LogModel *logModel;
  QSettings *settings;
  AboutDialog *about;
  Preferences *preferences;
  VideoModel *videoModel;
};
