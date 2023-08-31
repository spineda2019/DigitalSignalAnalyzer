#include "SignalInfoWindow.h"

#include "ui_SignalInfoWindow.h"

SignalInfoWindow::SignalInfoWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SignalInfoWindow()) {
  this->ui->setupUi(this);
}

SignalInfoWindow::~SignalInfoWindow() { delete this->ui; }
