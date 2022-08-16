#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <measmul.h>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_tick_timer()
  , m_value(0)
  , m_agilent(L"Agilent USB-GPIB", 0, 22, L"com4", L"0.0.0.0", L"0")
  , m_type_meas(type_meas_t::tm_value)
  , m_dc_enabled(false)
  , m_meas_value()
{
  ui->setupUi(this);

  m_agilent.enable(multimeter_mode_type_t::mul_mode_type_passive);
  qDebug() << "agilent enabled: " << m_agilent.enabled();

  if (m_agilent.enabled()) {
    m_meas_value.set_connect_multimeter(m_agilent.mxmultimeter());
  }

  connect(&m_tick_timer, &QTimer::timeout, this, &MainWindow::tick);
  m_tick_timer.start(100);
}

void MainWindow::tick()
{
  if (m_meas_value.is_multimeter_connected()) {
    m_meas_value.tick();

    if (m_meas_value.get_status_meas() != meas_status_t::meas_status_busy) {

      std::ostringstream str;
      str << std::fixed << std::setprecision(15) << m_value;
      ui->measured_label->setText(str.str().c_str());

      qDebug() << "start new measure" << (int)m_type_meas;
      m_meas_value.execute_meas(m_type_meas, &m_value);

    }
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_dc_button_clicked()
{
  m_dc_enabled = true;
  m_agilent.mxmultimeter()->set_dc();
}

void MainWindow::on_ac_button_clicked()
{
  m_dc_enabled = false;
  m_agilent.mxmultimeter()->set_ac();
}

void MainWindow::on_current_button_clicked()
{
  if (m_dc_enabled) {
    m_type_meas = type_meas_t::tm_current_dc;
  } else {
    m_type_meas = type_meas_t::tm_current_ac;
  }
  connect_to_multimeter(m_type_meas);
  m_agilent.mxmultimeter()->get_current(&m_value);
}

void MainWindow::on_voiltage_button_clicked()
{
  if (m_dc_enabled) {
    m_type_meas = type_meas_t::tm_volt_dc;
  } else {
    m_type_meas = type_meas_t::tm_volt_ac;
  }
  connect_to_multimeter(m_type_meas);
  m_agilent.mxmultimeter()->get_voltage(&m_value);
}

void MainWindow::on_set_range_button_clicked()
{
  double range = ui->range_spinbox->value();
  m_agilent.mxmultimeter()->set_range(m_type_meas, range);
}

void MainWindow::on_as_is_button_clicked()
{
  m_type_meas = type_meas_t::tm_value;
  connect_to_multimeter(m_type_meas);
  m_agilent.mxmultimeter()->get_value(&m_value);
}

bool MainWindow::connect_to_multimeter(type_meas_t a_meas_type)
{
  bool success = false;
  if (a_meas_type >= tm_first && a_meas_type <= tm_last) {
    multimeter_mode_type_t mode = a_meas_type == tm_value ? mul_mode_type_passive : mul_mode_type_active;
    m_agilent.enable(mode);
    if (m_agilent.enabled()) {
      if (m_meas_value.set_connect_multimeter(m_agilent.mxmultimeter())) {
        success = true;
      }
    }
  }
  return success;
}


void MainWindow::on_set_extra_commands_button_clicked()
{
//  send_commands = true;
  QString commands_str = ui->extra_commands_edit->text();
  QStringList commands_list = commands_str.split(";");

  vector<irs::string> commands;
  commands.reserve(commands_list.size());

  for (auto &cmd: commands_list) {
    commands.push_back(cmd.toStdString());
  }
//  m_meas_value.set_extra_commands(m_type_meas, commands);
}
