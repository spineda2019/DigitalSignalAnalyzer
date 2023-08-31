#include <QtWidgets/QApplication>
#include <iostream>

#include <QtGlobal>

#include "MainWidget.h"

int main(int argc, char* argv[]) {
  std::cout << "Here" << std::endl;
  QApplication app(argc, argv);
  app.setApplicationName("Digital Signal Analyzer");

  MainWidget mw;
  mw.setWindowTitle("Digital Signal Analyzer");
  mw.show();

  return app.exec();
}
