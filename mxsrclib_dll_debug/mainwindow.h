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

  void on_dc_button_clicked();
  void on_ac_button_clicked();
  void on_current_button_clicked();
  void on_voiltage_button_clicked();
  void on_set_range_button_clicked();

  void on_as_is_button_clicked();

  void on_set_extra_commands_button_clicked();

private:
  Ui::MainWindow *ui;
  QTimer m_tick_timer;

  double m_value;
  agilent_3458a_mxmultimeter_t m_agilent;

  type_meas_t m_type_meas;
  bool m_dc_enabled;
  value_meas_t m_meas_value;

  bool connect_to_multimeter(type_meas_t a_meas_type);
};
#endif // MAINWINDOW_H
