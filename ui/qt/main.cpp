#include <QApplication>

#include "controllers/main_window.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QApplication::setQuitOnLastWindowClosed(false);
  ui::MainWindow win;
  win.show();
  return QApplication::exec();
}
