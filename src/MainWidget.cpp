#include "MainWidget.h"

#include <QAudioFormat>
#include <QAudioInput>
#include <QErrorMessage>
#include <QFile>
#include <QFileDialog>
#include <QPointer>
#include <QTableWidget>

#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::MainWidget()),
      microphone_{new QAudioInput(this)},
      audio_destination_{new QFile(this)} {
  this->ui->setupUi(this);
  setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

  connect(this->ui->select_file, SIGNAL(pressed()), this,
          SLOT(on_select_file_clicked()));

  connect(this->ui->record_audio_request, SIGNAL(pressed()), this,
          SLOT(on_record_audio_request_clicked()));
}

MainWidget::~MainWidget() { delete this->ui; }

void MainWidget::on_select_file_clicked() {
  QString n_string = ui->n_data->text();
  QString sample_rate_string = ui->sample_rate->text();

  if (n_string.isEmpty() || sample_rate_string.isEmpty()) {
    ui->field_warning->setStyleSheet("color: red");
    ui->field_warning->setText("Make sure all field are filled out!");
    return;
  }

  // if fields are good, get rid of warning
  ui->field_warning->setText("");

  size_t n_data = n_string.toULongLong();
  size_t sample_rate_data = sample_rate_string.toULongLong();

  // set loading indicator and blank loaded settings
  this->ui->settings_view->setText("");
  this->ui->select_done->setStyleSheet("background-color: red");
  this->ui->confirmation->setText("Loading...");

  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Open Data File"), QDir::homePath(),
                                   tr("Data Files (*.csv *.txt)"));

  if (fileName.isEmpty()) {
    this->waves_ = nullptr;
    this->ui->select_done->setStyleSheet("background-color: yellow");
    this->ui->confirmation->setText("No CSV Loaded");
    disconnect(this->ui->run_command, SIGNAL(pressed()), this, SLOT(run()));
    return;
  }

  std::string file_path = fileName.toStdString();

  std::unique_ptr<read_file::Dataframe<float>> tmp(
      new read_file::Dataframe<float>(file_path, false));

  std::unique_ptr<read_file::Dataframe<float>> dsp =
      tmp->GetDSPData(n_data, sample_rate_data);

  this->waves_ = std::move(dsp);

  std::string settings_std_str =
      "CSV Loaded with n=" + n_string.toStdString() +
      " and sample_rate=" + sample_rate_string.toStdString();

  QString settings = QString(settings_std_str.c_str());

  this->ui->settings_view->setText(settings);

  if (file_path.length() > 0) {
    this->ui->select_done->setStyleSheet("background-color: green");
    connect(this->ui->run_command, SIGNAL(pressed()), this, SLOT(run()));
    this->ui->confirmation->setText("CSV Loaded!");
  }
}

void MainWidget::on_record_audio_request_clicked() {
  QString audio_dest = QFileDialog::getSaveFileName(
      this, tr("Save Audio File"), QDir::homePath(), tr("Audio File (*.wav)"));

  if (audio_dest.isEmpty()) {
    return;
  }

  this->audio_destination_->setFileName(audio_dest);
  this->audio_destination_->open(QIODevice::WriteOnly | QIODevice::Truncate);

  QAudioFormat* format(new QAudioFormat());
  format->setSampleRate(8000);                   // Change to user input
  format->setChannelCount(1);                    // Change to user input
  format->setSampleFormat(QAudioFormat::Float);  // Change to user input

  // stuff

  QErrorMessage* test(new QErrorMessage(this));
  test->showMessage("Hi");

  connect(this->microphone_, SIGNAL(stateChanged(QAudio::State)), this,
          SLOT(handleStateChanged(QAudio::State)));

  delete format;
}

void MainWidget::stop_recording() {}

void MainWidget::run() {
  if (this->ui->request_csv->isChecked()) {
    // Generate CSV
    QString dest_path = QFileDialog::getSaveFileName(
        this, tr("Save CSV File"), QDir::homePath(), tr("CSV File (*.csv)"));

    if (dest_path.isEmpty()) {
      return;
    } else {
      this->waves_->ToCsv(dest_path.toStdString());
    }
  }

  QPointer<QTableWidget> dsp_data(new QTableWidget(
      this->waves_->Height() + 1, this->waves_->Width(), nullptr));

  for (size_t col = 0; col < this->waves_->Width(); col++) {
    const std::string& header = this->waves_->GetHeader(col);

    dsp_data->setCellWidget(
        0, static_cast<int>(col),
        new QLabel(this->waves_->GetHeader(col).c_str(), dsp_data));
  }

  for (size_t row = 1; row <= this->waves_->Height(); row++) {
    for (size_t col = 0; col < this->waves_->Width(); col++) {
      const std::string& dsp_element =
          std::to_string(this->waves_->GetElement(row - 1, col));

      dsp_data->setCellWidget(static_cast<int>(row), static_cast<int>(col),
                              new QLabel(dsp_element.c_str(), dsp_data));
    }
  }

  dsp_data->show();
}
