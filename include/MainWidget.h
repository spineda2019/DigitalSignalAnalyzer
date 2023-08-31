#ifndef DIGITALSIGNALANALYZER_INCLUDE_MAINWIDGET_H
#define DIGITALSIGNALANALYZER_INCLUDE_MAINWIDGET_H

#include <QAudioInput>
#include <QFile>
#include <QPointer>
#include <QWidget>
#include <memory>
#include <string>

#include "Dataframe.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
};
QT_END_NAMESPACE

class MainWidget : public QWidget {
  Q_OBJECT

 public:
  MainWidget(QWidget *parent = nullptr);
  ~MainWidget();

 private slots:
  void on_select_file_clicked();
  void on_record_audio_request_clicked();
  void stop_recording();
  void run();

 private:
  Ui::MainWidget *ui;
  std::unique_ptr<read_file::Dataframe<float>> waves_;
  QPointer<QAudioInput> microphone_;
  QPointer<QFile> audio_destination_;
};

#endif  // DIGITALSIGNALANALYZER_INCLUDE_MAINWIDGET_H
