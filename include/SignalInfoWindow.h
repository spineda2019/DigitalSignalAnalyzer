#ifndef DIGITALSIGNALANALYZER_INCLUDE_SIGNALINFOWINDOW_H
#define DIGITALSIGNALANALYZER_INCLUDE_SIGNALINFOWINDOW_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class SignalInfoWindow;
};
QT_END_NAMESPACE

class SignalInfoWindow : public QMainWindow {
  Q_OBJECT

 public:
  SignalInfoWindow(QWidget *parent = nullptr);
  ~SignalInfoWindow();

 private:
  Ui::SignalInfoWindow *ui;
};

#endif  // DIGITALSIGNALANALYZER_INCLUDE_SIGNALINFOWINDOW_H
