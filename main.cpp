#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("mexus");
  QCoreApplication::setApplicationName("Pass-GUI");

  MainWindow w;

  if (a.arguments().indexOf("--tray") == -1) {
    w.show();
  }

  return a.exec();
}
