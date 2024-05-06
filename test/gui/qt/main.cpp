#include <QApplication>

#include "main/main_window.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QApplication::setQuitOnLastWindowClosed(false);
  TestQt::MainWindow win;
  win.show();
  return QApplication::exec();
}
