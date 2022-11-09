#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mxsrclib_dll_global.h"
#include <measmul.h>
#include <QDebug>


MXSRCLIB_DLLEXPORT int connect_to_agilent_3458a(size_t a_meas_type, const wchar_t* a_name, int a_gpib_index,
  int a_gpib_address, const wchar_t* a_com_name, const wchar_t* a_ip_str, const wchar_t *a_port_str);
MXSRCLIB_DLLEXPORT int multimeter_set_config(const char* a_config_string,
  double a_apply_delay_s);
MXSRCLIB_DLLEXPORT int multimeter_start_measure(size_t a_meas_type, double *a_value);
MXSRCLIB_DLLEXPORT int multimeter_get_status();
MXSRCLIB_DLLEXPORT void multimeter_tick();


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_tick_timer()
  , m_value(0)
  , m_type_meas(type_meas_t::tm_value)
  , m_dc_enabled(false)
  , m_meas_value()
  , m_measure_started(false)
{
  ui->setupUi(this);

  qDebug() << "Connect to Agilent...";
  connect_to_agilent_3458a(tm_value, L"Agilent USB-GPIB", 0, 22, L"com3", L"0.0.0.0", L"0");
  qDebug() << "Connected";

  connect(&m_tick_timer, &QTimer::timeout, this, &MainWindow::tick);
  m_tick_timer.start(100);
}

void MainWindow::tick()
{
  multimeter_tick();
  static meas_status_t status_prev = meas_status_error;
  meas_status_t status = static_cast<meas_status_t>(multimeter_get_status());
  if (status_prev != status) {
    status_prev = status;

    switch (status) {
      case meas_status_success: {
        ui->status_label->setText("Success");

        if (m_measure_started) {
          m_measure_started = false;

          std::ostringstream str;
          str << std::fixed << std::setprecision(15) << m_value;
          ui->measured_label->setText(str.str().c_str());
        }

      } break;
      case meas_status_busy: {
        ui->status_label->setText("Busy");
      } break;
      case meas_status_error: {
        ui->status_label->setText("Error");
      } break;
    }
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_set_extra_commands_button_clicked()
{
  QString commands_str = ui->extra_commands_edit->text();
  if (!commands_str.isEmpty()) {
    qDebug() << "Send" << commands_str;
    multimeter_set_config(commands_str.toStdString().c_str(), 2);
  }
}

void MainWindow::on_dcv_button_clicked()
{
  QString config = "DCV " + QString::number(ui->range_spinbox->value());
  config.replace(",", ".");
  qDebug() << "Send" << config;
  multimeter_set_config(config.toStdString().c_str(), 2);
}

void MainWindow::on_dci_button_clicked()
{
  QString config = "DCI " + QString::number(ui->range_spinbox->value());
  config.replace(",", ".");
  qDebug() << "Send" << config;
  multimeter_set_config(config.toStdString().c_str(), 2);
}

void MainWindow::on_acv_button_clicked()
{
  QString config = "ACV " + QString::number(ui->range_spinbox->value());
  config.replace(",", ".");
  qDebug() << "Send" << config;
  multimeter_set_config(config.toStdString().c_str(), 2);
}

void MainWindow::on_aci_button_clicked()
{
  QString config = "ACI " + QString::number(ui->range_spinbox->value());
  config.replace(",", ".");
  qDebug() << "Send" << config;
  multimeter_set_config(config.toStdString().c_str(), 2);
}

void MainWindow::on_make_measure_button_clicked()
{
  qDebug() << "Start measure";
  multimeter_start_measure(tm_value, &m_value);
  m_measure_started = true;
}
