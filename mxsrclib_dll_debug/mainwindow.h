#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "irsdevices.h"
#include "multimeter_measure.h"
#include "agilent_3458a_mxmultimeter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void tick();

  void on_set_extra_commands_button_clicked();
  void on_dcv_button_clicked();
  void on_dci_button_clicked();
  void on_acv_button_clicked();
  void on_aci_button_clicked();
  void on_make_measure_button_clicked();

private:
  Ui::MainWindow *ui;
  QTimer m_tick_timer;

  double m_value;

  type_meas_t m_type_meas;
  bool m_dc_enabled;
  value_meas_t m_meas_value;
  bool m_measure_started;

  bool connect_to_multimeter(type_meas_t a_meas_type);
};
#endif // MAINWINDOW_H
